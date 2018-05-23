
#ifndef PING_PONG_H
# define PING_PONG_H

# include <Python.h>
# include <math.h>
# include "numpy/arrayobject.h"

# define MAGIC_WAND_TOLERANCE 20.0
# define GROUP_BY_CLOSEST 0

typedef struct		s_opencv_image
{
	unsigned char	*pixels;
	int				height;
	int				width;
	int				y_decal;
	int				x_decal;
	int				z_decal;
}					t_opencv_image;

typedef struct		s_list
{
	void			*elts;
	int				size;
	int				len;
	size_t			elt_size;
}					t_list;

typedef struct		s_magic_wand_mask
{
	int				*selection_ind_mask;
	float			*color_distances;
	int				selection_ind;
	t_list			pixels_to_explore;
}					t_magic_wand_mask;

typedef struct		s_pixel_to_explore
{
	int				y;
	int				x;
	unsigned char	to_compare_b;
	unsigned char	to_compare_g;
	unsigned char	to_compare_r;
}					t_pixel_to_explore;

typedef struct		s_selections
{
	int				*pixels;
	int				*end_of_selections;
	int				*tmp;
	int				nb;
}					t_selections;

t_magic_wand_mask		g_wand_mask;
t_selections				g_selections;

void				init_list(t_list *list, size_t elt_size);
void				*new_elt(t_list *lst);
void				remove_elt(t_list *lst, char *addr);
void				*pop_elt(t_list *lst);
void				free_list(t_list *lst);
void				get_possible_balls(t_opencv_image *image, t_selections *selections, char already_malloced);
void				set_rgb(t_opencv_image *image, int y, int x, int r, int g, int b);
unsigned char		get_from_image(t_opencv_image *image, int y, int x, int z);

#endif
