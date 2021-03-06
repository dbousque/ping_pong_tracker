

#include "ping_pong.h"

/*
MAGIC_WAND_TOLERANCE = 50.0

def color_distance(c1, c2):
  r1, g1, b1 = int(c1[0]), int(c1[1]), int(c1[2])
  r2, g2, b2 = int(c2[0]), int(c2[1]), int(c2[2])
  return math.sqrt((r1 - r2) ** 2 + (g1 - g2) ** 2 + (b1 - b2) ** 2)

def explore_magic_wand_at(image, y, x, mask, selection_id):
  pixels_to_explore = []
  pixels_to_explore = [[y, x, image[y, x]]]
  while len(pixels_to_explore) > 0:
    y, x, color_to_compare = pixels_to_explore.pop()
    if y < 0 or x < 0 or y >= image.shape[0] or x >= image.shape[1]:
      continue
    distance = color_distance(image[y, x], color_to_compare)
    target_distance = MAGIC_WAND_TOLERANCE if mask[y, x, 0] == 0 else mask[y, x, 1]
    if distance < target_distance:
      mask[y, x, 0] = selection_id
      mask[y, x, 1] = distance
      color = image[y, x]
      pixels_to_explore.append([y + 1, x, color])
      pixels_to_explore.append([y - 1, x, color])
      pixels_to_explore.append([y, x + 1, color])
      pixels_to_explore.append([y, x - 1, color])

def magic_wand(image):
  mask = np.zeros((image.shape[0], image.shape[1], 2))
  selection_id = 1
  for y in range(image.shape[0]):
    for x in range(image.shape[1]):
      if mask[y, x, 0] != 0:
        continue
      explore_magic_wand_at(image, y, x, mask, selection_id)
      selection_id += 1

  selections = [[]] * (selection_id - 1)
  for y in range(image.shape[0]):
    for x in range(image.shape[1]):
      selections[int(mask[y, x, 0]) - 1].append((y, x))
  return selections

def is_possible_ball(selection):
  return random.randint(0, 5) == 0

 selections = magic_wand(image)
  print('selections done')
  possible_balls = list(filter(is_possible_ball, selections))
*/


int		millis_since(struct timeval *start)
{
	struct timeval	end;

	gettimeofday(&end, NULL);
	return (1000 * (end.tv_sec - start->tv_sec) + (end.tv_usec - start->tv_usec) / 1000);
}

void	print_time_taken(struct timeval *start, char *before, char *after)
{
	int		time_taken;

	time_taken = millis_since(start);
	printf("%s%d ms%s", before, time_taken, after);
}

float	get_color_distance(int b, int g, int r, t_pixel_to_explore *pixel)
{
	return (sqrt(
		pow(b - pixel->to_compare_b, 2) +
		pow(g - pixel->to_compare_g, 2) +
		pow(r - pixel->to_compare_r, 2)
	));
}

void	explore_magic_wand_at(t_opencv_image *image, int y, int x, t_magic_wand_mask *wand_mask)
{
	t_pixel_to_explore	*pixel;
	float				distance;
	float				target_distance;
	int					b;
	int					g;
	int					r;
	int					p_y;
	int					p_x;
	int					nb_in_selection;
	struct timeval		start;

	gettimeofday(&start, NULL);
	nb_in_selection = 0;
	wand_mask->pixels_to_explore.len = 0;
	pixel = new_elt(&(wand_mask->pixels_to_explore));
	pixel->y = y;
	pixel->x = x;
	pixel->to_compare_b = get_from_image(image, y, x, 0);
	pixel->to_compare_g = get_from_image(image, y, x, 1);
	pixel->to_compare_r = get_from_image(image, y, x, 2);
	while (wand_mask->pixels_to_explore.len > 0)
	{
		pixel = pop_elt(&(wand_mask->pixels_to_explore));
		p_y = pixel->y;
		p_x = pixel->x;
		if (p_y < 0 || p_x < 0 || p_y >= image->height || p_x >= image->width)
			continue ;
		if (!GROUP_BY_CLOSEST && wand_mask->selection_ind_mask[(p_y * image->width) + p_x] != -1)
			continue ;
		b = get_from_image(image, p_y, p_x, 0);
		g = get_from_image(image, p_y, p_x, 1);
		r = get_from_image(image, p_y, p_x, 2);
		distance = get_color_distance(b, g, r, pixel);
		target_distance = MAGIC_WAND_TOLERANCE;
		if (wand_mask->selection_ind_mask[(p_y * image->width) + p_x] != -1)
			target_distance = wand_mask->color_distances[(p_y * image->width) + p_x];
		if (distance >= target_distance)
			continue ;
		wand_mask->selection_ind_mask[(p_y * image->width) + p_x] = wand_mask->selection_ind;
		wand_mask->color_distances[(p_y * image->width) + p_x] = distance;
		nb_in_selection++;


		// WARNING HERE !!!
		//if (nb_in_selection > 80 * 80)
		//	return ;
		// WARNING HERE !!!

		
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = p_y - 1, .x = p_x, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = p_y + 1, .x = p_x, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = p_y, .x = p_x - 1, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = p_y, .x = p_x + 1, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
	}
	if (millis_since(&start) > 2)
	{
		printf("more than 2 millis, n %d\n", nb_in_selection);
		print_time_taken(&start, "", "\n");
	}
}

void	make_magic_wand_mask(t_opencv_image *image, t_magic_wand_mask *wand_mask)
{
	int		y;
	int		x;

	wand_mask->selection_ind = 0;
	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			if (wand_mask->selection_ind_mask[(y * image->width) + x] != -1)
			{
				x++;
				continue ;
			}
			explore_magic_wand_at(image, y, x, wand_mask);
			wand_mask->selection_ind++;
			x++;
		}
		y++;
	}
}

void	group_selections(t_opencv_image *image, t_magic_wand_mask *wand_mask, t_selections *selections)
{
	int		y;
	int		x;
	int		tmp_selection_ind;
	int		start_offset;

	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			tmp_selection_ind = wand_mask->selection_ind_mask[(y * image->width) + x];
			selections->end_of_selections[tmp_selection_ind]++;
			x++;
		}
		y++;
	}
	y = 1;
	while (y < selections->nb)
	{
		selections->end_of_selections[y] += selections->end_of_selections[y - 1];
		y++;
	}
	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			tmp_selection_ind = wand_mask->selection_ind_mask[(y * image->width) + x];
			start_offset = 0;
			if (tmp_selection_ind > 0)
				start_offset = selections->end_of_selections[tmp_selection_ind - 1];
			selections->pixels[(start_offset + selections->tmp[tmp_selection_ind]) * 2] = y;
			selections->pixels[(start_offset + selections->tmp[tmp_selection_ind]) * 2 + 1] = x;
			selections->tmp[tmp_selection_ind]++;
			x++;
		}
		y++;
	}
}

char	more_or_less_square(int *pixels, int length)
{
	int		top;
	int		bottom;
	int		left;
	int		right;
	int		i;
	float	ratio;

	top = pixels[0];
	bottom = pixels[0];
	left = pixels[1];
	right = pixels[1];
	i = 0;
	while (i < length)
	{
		if (pixels[i * 2] < top)
			top = pixels[i * 2];
		if (pixels[i * 2] > bottom)
			bottom = pixels[i * 2];
		if (pixels[i * 2 + 1] < left)
			left = pixels[i * 2 + 1];
		if (pixels[i * 2 + 1] > right)
			right = pixels[i * 2 + 1];
		i++;
	}
	if (left > 810 && left < 890)
		printf("%d %d %d %d\n", top, bottom, left, right);
	ratio = ((float)(bottom - top + 1)) / (right - left + 1);
	if (left > 810 && left < 890)
		printf("ratio : %.2f\n", ratio);
	if (ratio < 0.8 || ratio > 1.2)
		return (0);
	if (((float)((bottom - top + 1) + (right - left + 1))) / 2  > 80)
		return (0);
	return (1);
}

char	more_or_less_round(int *pixels, int length)
{
	
}

char	is_possible_ball(t_opencv_image *image, int *pixels, int length)
{
	if (length > 80 * 80)
		return (0);
	if (length < 10 * 10)
		return (0);
	if (!more_or_less_square(pixels, length))
		return (0);
	return (1);
}

void	find_possible_ball_selections(t_opencv_image *image, t_selections *selections)
{
	int		i;
	int		start_offset;
	int		selection_length;

	i = 0;
	while (i < selections->nb)
	{
		start_offset = 0;
		if (i > 0)
			start_offset = selections->end_of_selections[i - 1];
		selection_length = selections->end_of_selections[i] - start_offset;
		selections->tmp[i] = 0;
		if (is_possible_ball(image, selections->pixels + (start_offset * 2), selection_length))
			selections->tmp[i] = 1;
		i++;
	}
}

void	get_possible_balls(t_opencv_image *image, t_selections *selections, char already_malloced)
{
	int					i;
	struct timeval		start_time;

	gettimeofday(&start_time, NULL);
	if (!already_malloced)
	{
		g_wand_mask.selection_ind_mask = (int*)malloc(sizeof(int) * (image->width * image->height));
		g_wand_mask.color_distances = (float*)malloc(sizeof(float) * (image->width * image->height));
		selections->pixels = (int*)malloc(sizeof(int) * 2 * (image->width * image->height));
		init_list(&(g_wand_mask.pixels_to_explore), sizeof(t_pixel_to_explore));
	}
	i = 0;
	while (i < image->width * image->height)
	{
		g_wand_mask.selection_ind_mask[i] = -1;
		i++;
	}
	print_time_taken(&start_time, "after init : ", "\n");
	make_magic_wand_mask(image, &g_wand_mask);
	print_time_taken(&start_time, "after make_magic_wand_mask : ", "\n");
	selections->nb = g_wand_mask.selection_ind;
	selections->end_of_selections = (int*)malloc(sizeof(int) * (selections->nb));
	selections->tmp = (int*)malloc(sizeof(int) * (selections->nb));
	i = 0;
	while (i < selections->nb)
	{
		selections->end_of_selections[i] = 0;
		selections->tmp[i] = 0;
		i++;
	}
	group_selections(image, &g_wand_mask, selections);
	// selection->pixels contains [10, 10, 11, 10, 40, 40, 41, 40]
	// selection->end_of_selections contains [2, 4]
	//free(g_wand_mask.selection_ind_mask);
	//free(g_wand_mask.color_distances);
	//free_list(&(g_wand_mask.pixels_to_explore));
	find_possible_ball_selections(image, selections);
	print_time_taken(&start_time, "total : ", "\n");
	// selection->tmp now contains [0, 1]
}