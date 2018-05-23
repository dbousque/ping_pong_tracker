

#include "ping_pong.h"

static void		write_in_image(t_opencv_image *image, int y, int x, int z, unsigned char val)
{
	*(image->pixels + (y * image->y_decal) + (x * image->x_decal) + (z * image->z_decal)) = val;
}

void	set_rgb(t_opencv_image *image, int y, int x, int r, int g, int b)
{
	write_in_image(image, y, x, 0, b);
	write_in_image(image, y, x, 1, g);
	write_in_image(image, y, x, 2, r);
}

unsigned char	get_from_image(t_opencv_image *image, int y, int x, int z)
{
	return *(image->pixels + (y * image->y_decal) + (x * image->x_decal) + (z * image->z_decal));
}

static PyObject	*get_possible_balls_wrapper(PyObject *self, PyObject *args)
{
	PyArrayObject 	*array;
	t_opencv_image	image;
	char			*error_msg;
	PyObject		*res_balls;
	PyObject		*tmp_ball;
	PyObject		*tmp_pixel;
	int				i;
	int				start_offset;
	int				selection_length;
	int				ind;
	PyObject		*y;
	PyObject		*x;
	char			already_malloced;

	array = NULL;
	if (!PyArg_ParseTuple(args, "O!", &PyArray_Type, &array))
		return NULL;
	if (array->nd != 3 || array->descr->type_num != PyArray_UBYTE)
	{
		PyErr_SetString(PyExc_ValueError, "invalid image");
		return NULL;
	}
	if (array->dimensions[2] != 3)
	{
		PyErr_SetString(PyExc_ValueError, "image must be RGB");
		return NULL;
	}
	image.pixels = (unsigned char*)array->data;
	image.height = array->dimensions[0];
	image.width = array->dimensions[1];
	image.y_decal = array->strides[0];
	image.x_decal = array->strides[1];
	image.z_decal = array->strides[2];
	error_msg = NULL;
	already_malloced = 0;
	if (g_wand_mask.selection_ind_mask)
		already_malloced = 1;
	get_possible_balls(&image, &g_selections, already_malloced);
	if (!(res_balls = PyList_New(0)))
		return NULL;
	i = 0;
	while (i < g_selections.nb)
	{
		if (g_selections.tmp[i] != 1)
		{
			i++;
			continue ;
		}
		if (!(tmp_ball = PyList_New(0)))
			return NULL;
		start_offset = 0;
		if (i > 0)
			start_offset = g_selections.end_of_selections[i - 1];
		selection_length = g_selections.end_of_selections[i] - start_offset;
		ind = start_offset;
		while (ind < start_offset + selection_length)
		{
			y = PyLong_FromLong(g_selections.pixels[ind * 2]);
			x = PyLong_FromLong(g_selections.pixels[ind * 2 + 1]);
			if (!(tmp_pixel = PyTuple_Pack(2, y, x)))
				return NULL;
			PyList_Append(tmp_ball, tmp_pixel);
			ind++;
		}
		PyList_Append(res_balls, tmp_ball);
		i++;
	}
	//free(selections.pixels);
	free(g_selections.end_of_selections);
	free(g_selections.tmp);
	return res_balls;
}

static PyMethodDef  PingPongMethods[] =
{
	{
		"get_possible_balls",
		get_possible_balls_wrapper,
		METH_VARARGS,
		"returns a list of possible ping pong balls in the image"
	},
	{NULL, NULL, 0, NULL}
};

/* module initialization */
static struct PyModuleDef cModPyDem =
{
	PyModuleDef_HEAD_INIT,
	"ping_pong", "documentation",
	-1,
	PingPongMethods
};

PyMODINIT_FUNC
PyInit_ping_pong(void)
{
	import_array();
	g_wand_mask.selection_ind_mask = NULL;
	return PyModule_Create(&cModPyDem);
}