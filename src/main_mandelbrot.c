#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>
#include <X11/keysym.h>
#include <X11/X.h>
#include "../lib/libft/ft_printf.h"

//too many functions. should group in different source files. Something like:
//1. main.c;
//2. *fractals.c
//3. *events.c
//4. *render.c and pixel_put (or these put together with main)
//5. all functions for operations with complex numbers before recursion

//macros should probably also go on an h file
# define WINDOW_WIDTH 1000
# define WINDOW_HEIGHT 600
# define MLX_ERROR 1
# define RED_PIXEL 0xFF0000
# define GREEN_PIXEL 0x00FF00
# define WHITE_PIXEL 0xFFFFFF

# define MAX_ITER 1000

//struct definitions should go in an h file.
typedef struct	s_img
{
	void	*mlx_img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct	s_data
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	img;
}	t_data;

typedef struct	s_complex
{
	double	real;
	double	imaginary;
}	t_complex;

/*These function prototypes are already in the mlx 
 * header files,so no need to include them:
 * void	*mlx_new_image(void *mlx_ptr, int width, int height);
 * char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);
 * int	mlx_put_image_to_window(t_xvar *xvar,t_win_list *win,t_img *img, int x,int y);*/

/* Use this function(renders image first and then pushes it to the window) 
 * instead of mlx_pixel_put (draws pixel on window directly without any 
 * delay causing a flickering effect).
 * 1. Locate a pixel (addr is base, i.e., 0,0; y * img->line_len skips down y rows
 * and x * (bpp / 8) skips x pixels into the row)
 * 2. Color the pixel*/
void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

t_complex	ft_fractal_new_z(t_complex t_z, t_complex t_c)
{
	t_complex	t_z_squared;
	t_complex	t_z_new;

	t_z_squared.real = (t_z.real * t_z.real) - (t_z.imaginary * t_z.imaginary);
	t_z_squared.imaginary = 2 * t_z.real * t_z.imaginary;
	t_z_new.real = t_z_squared.real + t_c.real;
	t_z_new.imaginary = t_z_squared.imaginary + t_c.imaginary;
	return (t_z_new);
}

double	ft_fractal_absolute_z(t_complex t_z)
{
	double	absolute_z;

	absolute_z = (t_z.real * t_z.real) + (t_z.imaginary * t_z.imaginary);
	return (absolute_z);
}

int	ft_fractal_recursion(t_complex t_z, t_complex t_c, int count_iter)
{
	if (ft_fractal_absolute_z(t_z) > 4.0)
		return (count_iter);
	if (count_iter >= MAX_ITER)
		return (count_iter);
	t_z = ft_fractal_new_z(t_z, t_c);	
	return (ft_fractal_recursion(t_z, t_c, count_iter + 1)); 
}

double	ft_fractal_escape(t_complex t_c)
{
	t_complex	t_z;

	t_z.real = 0;
	t_z.imaginary = 0;
	return (ft_fractal_recursion(t_z, t_c, 0));
}

/*Include parameter for set type (i.e., argv[1]) 
 * because each set type will have a different RE_START and RE_END since they 
 * extend differently accross the complex plane.*/
int	ft_fractal_color(int x, int y) 
{
	//all this resizing part at the begining i could move to another function
	t_complex	t_start;
	t_complex	t_end;
	t_complex	t_dimension;
	t_complex	t_c;
	int			count_iter;
	int			color;

	//mandelbrot set region x=-2,1 and y=-1.5,1.5:
	t_start.real = -2.0;
	t_end.real = 1.0;
	t_start.imaginary = -1.5;
	t_end.imaginary = 1.5;
	t_dimension.real = t_end.real - t_start.real;
	t_dimension.imaginary = t_end.imaginary - t_start.imaginary;
	t_c.real = t_start.real + ((double)x / WINDOW_WIDTH * t_dimension.real);
	t_c.imaginary = t_start.imaginary + ((double)y / WINDOW_HEIGHT * t_dimension.imaginary);
	count_iter = ft_fractal_escape(t_c);
	//ft_printf("c (%.4f, %.4f) , escapes at count_iter = %i\n", t_c.real, t_c.imaginary, count_iter);
	//here go instructions for coloring based on count_iter (the escape time algorithm.)
	//all else (i.e. the resizing part) should go on separate function.
	if (count_iter == MAX_ITER)
		color = 0x000000;
	else if (count_iter > 500)
		color = 0x00ff00;
	else if (count_iter > 92)
		color = 0xff0000;
	else if (count_iter > 60)
		color = 0x0000ff;
	else if (count_iter > 30)
		color = 0xa65c00;
	else if (count_iter > 10)
		color = 0x00ff00;
	else if (count_iter > 8)
		color = 0xff0000;
	else if (count_iter > 6)
		color = 0xff00ff;
	else if (count_iter > 4)
		color = 0xff0000;
	else if (count_iter > 2)
		color = 0x0000ff;
	else
		color = 0x00ff00;
	return (color);
}

void	render_fractal(t_img *img)
{
	int	i;
	int	j;
	int	color;

	i = 0;
	while (i < WINDOW_HEIGHT)
	{
		j = 0;
		while (j < WINDOW_WIDTH)
		{
			//here call a function x that calls the recursive function y.
			//Function y should return n = number of iterations when c escapes,
			//or n = max_iterations if c doesnt escape after max_iterations.
			//Then function c assigns a color based on y's return, and returns to
			//this function.

			//Very likely will have to recalibrate origin of the window 
			//or where pixels are put, because in the minilibx window,
			//the origin (0,0) is a the top left corner, but the mandelbrot set
			//has its origin at (-.5, 0). I could do this adjustment in the
			//ft_fractal_color function, or at the ft_fractal_escape function.
			color = ft_fractal_color(j, i);
			img_pix_put(img, j++, i, color);
		}
		i ++;
	}
}

int	render(t_data *data)
{
	if (data->win_ptr == NULL)
		return (1);
	render_fractal(&data->img);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
	return (0);
}

int	handle_keypress(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
	{
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		data->win_ptr = NULL;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 2 || argc > 3) // > 3 ?
	{
		ft_printf("Invalid input. Enter a fractal set (mandelbrot / julia) + additional xxx parameter if the julia set is chosen\n");
		return (-1);
	}
	(void)argv;
	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "My first window!");
	if (data.win_ptr == NULL)
		return (free(data.win_ptr), MLX_ERROR);

	/*Setup hooks (i.e., callbacks: functions to be executed later,
	 * when certain events occur inside the loop):*/
	//mlx_new_image returns the address of the image stored in shared memory(mlx_img):
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);
	//mlx_get_data_addr returns a char*. The bpp,line_len and endian are modified 
	//by the function itself (bpp is usually 32, because each pixel has 4 bytes.
	//	line_len (if there's no padding) is the number of bytes per row, i.e., 
	//	image_width * (bpp / 8), and endian depends on the machine (42 machines are LE) 
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);
	//mlx_loop_hook: for always-running logic, like animation or redrawing:
 	mlx_loop_hook(data.mlx_ptr, &render, &data);
	//mlx_hook: for specific events, like key press, mouse movement, window resize:
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);

	/*mlx_loop: Starts the event main loop. It runs forever (until you manually destroy 
	 * the window or exit), waiting for user input (keyboard/mouse) or internal events, 
	 * and dispatches them to your hook functions (like mlx_hook() or mlx_loop_hook()).*/
	mlx_loop(data.mlx_ptr);

	/*exits the loop when there's no window left or escape key is pressed.*/
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
}

