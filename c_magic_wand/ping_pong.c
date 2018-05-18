

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


void	set_colors(t_opencv_image *image)
{
	int		x;
	int		y;

	x = 100;
	while (x < 200)
	{
		y = 100;
		while (y < 200)
		{
			set_rgb(image, x, y, 255, 0, 0);
			y++;
		}
		x++;
	}
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
		if (pixel->y < 0 || pixel->x < 0 || pixel->y >= image->height || pixel->x >= image->width)
			continue ;
		b = get_from_image(image, pixel->y, pixel->x, 0);
		g = get_from_image(image, pixel->y, pixel->x, 1);
		r = get_from_image(image, pixel->y, pixel->x, 2);
		distance = get_color_distance(b, g, r, pixel);
		target_distance = MAGIC_WAND_TOLERANCE;
		if (wand_mask->selection_ind_mask[(y * image->width) + x] != -1)
			target_distance = wand_mask->color_distances[(y * image->width) + x];
		if (distance >= target_distance)
			continue ;
		printf("OK!\n");
		wand_mask->selection_ind_mask[(y * image->width) + x] = wand_mask->selection_ind;
		wand_mask->color_distances[(y * image->width) + x] = distance;
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = y - 1, .x = x, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = y + 1, .x = x, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = y, .x = x - 1, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
		pixel = new_elt(&(wand_mask->pixels_to_explore));
		*pixel = (t_pixel_to_explore){ .y = y, .x = x + 1, .to_compare_b = b, .to_compare_g = g, .to_compare_r = r };
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

char	is_possible_ball(t_opencv_image *image, int *pixels, int length)
{
	if (length > 40 * 40)
		return (0);
	return (rand() % 5 == 0);
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
		if (is_possible_ball(image, selections->pixels + start_offset, selection_length))
			selections->tmp[i] = 1;
		i++;
	}
}

void	get_possible_balls(t_opencv_image *image, t_selections *selections)
{
	t_magic_wand_mask	wand_mask;
	int					i;

	wand_mask.selection_ind_mask = (int*)malloc(sizeof(int) * (image->width * image->height));
	wand_mask.color_distances = (float*)malloc(sizeof(float) * (image->width * image->height));
	selections->pixels = (int*)malloc(sizeof(int) * 2 * (image->width * image->height));
	init_list(&(wand_mask.pixels_to_explore), sizeof(t_pixel_to_explore));
	i = 0;
	while (i < image->width * image->height)
	{
		wand_mask.selection_ind_mask[i] = -1;
		i++;
	}
	make_magic_wand_mask(image, &wand_mask);
	printf("%d\n", image->width * image->height);
	printf("%d\n", wand_mask.selection_ind + 1);
	selections->nb = wand_mask.selection_ind + 1;
	selections->end_of_selections = (int*)malloc(sizeof(int) * (selections->nb));
	selections->tmp = (int*)malloc(sizeof(int) * (selections->nb));
	i = 0;
	while (i < selections->nb)
	{
		selections->end_of_selections[i] = 0;
		selections->tmp[i] = 0;
		i++;
	}
	group_selections(image, &wand_mask, selections);
	// selection->pixels contains [10, 10, 11, 10, 40, 40, 41, 40]
	// selection->end_of_selections contains [2, 4]
	free(wand_mask.selection_ind_mask);
	free(wand_mask.color_distances);
	free_list(&(wand_mask.pixels_to_explore));
	find_possible_ball_selections(image, selections);
	// selection->tmp now contains [0, 1]
	set_colors(image);
}