#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>
#include <X11/keysym.h>
#include <X11/X.h>
#include <math.h>
#include "../lib/libft/ft_printf.h"

//too many functions. should group in different source files. Something like:
//1. main.c;
//2. *fractals.c
//3. *events.c
//4. *render.c and pixel_put (or these put together with main)
//5. all functions for operations with complex numbers before recursion

//macros should probably also go on an h file
# define WINDOW_WIDTH 900
# define WINDOW_HEIGHT 900
# define MLX_ERROR 1
# define RED_PIXEL 0xFF0000
# define GREEN_PIXEL 0x00FF00
# define WHITE_PIXEL 0xFFFFFF

# define MAX_ITER 1000

# define MAX_CALCULATIONS_PER_FRAME 10000000

#define SCROLL_UP 4
#define SCROLL_DOWN 5

#define ZOOM_IN_FACTOR 0.8
#define ZOOM_OUT_FACTOR 1.2

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
	int		current_pixel_x;
	int		current_pixel_y;
	int		render_pass_max_iter;
	int		pixels_rendered_this_frame;
	int		is_rendering;
	long double	re_start;
	long double	re_end;
	long double	im_start;
	long double	im_end;
}	t_data;

typedef struct	s_complex
{
	long double	real;
	long double	imaginary;
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

int	ft_fractal_interpolate_color(int color1, int color2, long double t)
{
	int	r1 = (color1 >> 16) & 0xFF;
	int	g1 = (color1 >> 8) & 0xFF;
	int	b1 = color1 & 0xFF;

	int	r2 = (color2 >> 16) & 0xFF;
	int	g2 = (color2 >> 8) & 0xFF;
	int	b2 = color2 & 0xFF;

	int	r = (int)(r1 + t * (r2 - r1));
	int	g = (int)(g1 + t * (g2 - g1));
	int	b = (int)(b1 + t * (b2 - b1));

	return ((r << 16) | (g << 8) | b);
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

long double	ft_fractal_absolute_z(t_complex t_z)
{
	long double	absolute_z;

	absolute_z = (t_z.real * t_z.real) + (t_z.imaginary * t_z.imaginary);
	return (absolute_z);
}

long double	ft_fractal_smooth(int count_iter, t_complex t_z)
{
	long double	log_zn;
	long double	count_iter_smooth;
	long double	absolute_z;

	absolute_z = ft_fractal_absolute_z(t_z);
	log_zn = 0.5 * log(absolute_z);
	count_iter_smooth = (long double)count_iter + 1.0 - (log(log_zn) / log(2.0));
	return (count_iter_smooth);
}

long double	ft_fractal_recursion(t_complex t_z, t_complex t_c, int count_iter, t_data *data)
{
	data->pixels_rendered_this_frame += 7;
	if (data->pixels_rendered_this_frame >= MAX_CALCULATIONS_PER_FRAME)
		return ((long double)MAX_ITER);
	if (ft_fractal_absolute_z(t_z) > 4.0)
		return (ft_fractal_smooth(count_iter, t_z));
	if (count_iter == data->render_pass_max_iter)
		return (count_iter);
	t_z = ft_fractal_new_z(t_z, t_c);
	return (ft_fractal_recursion(t_z, t_c, count_iter + 1, data));
}

long double	ft_fractal_escape(t_complex t_c, t_data *data)
{
	t_complex	t_z;

	t_z.real = 0;
	t_z.imaginary = 0;
	return (ft_fractal_recursion(t_z, t_c, 0, data));
}

/*Include parameter for set type (i.e., argv[1]) 
 * because each set type will have a different RE_START and RE_END since they 
 * extend differently accross the complex plane.*/
int	ft_fractal_color(t_complex t_c, t_data *data) 
{
	long double		count_iter_smooth;
	int			color;

	count_iter_smooth = ft_fractal_escape(t_c, data);
	if (count_iter_smooth == (long double)data->render_pass_max_iter)
		color = 0x000000;
	else
	{
		long double color_value = fmod(count_iter_smooth * 0.1, 1.0);
		int	color1 = 0x0000FF;
		int	color2 = 0x00FF00;
		color = ft_fractal_interpolate_color(color1, color2, color_value);
	}
	return (color);
}

//mandelbrot set region x=-2,1 and y=-1.5,1.5:
t_complex	ft_fractal_mapping(int x, int y, t_data *data)
{
	t_complex	t_dimension;
	t_complex	t_c;

	t_dimension.real = data->re_end - data->re_start;
	t_dimension.imaginary = data->im_end - data->im_start;
	t_c.real = data->re_start + ((long double)x / WINDOW_WIDTH * t_dimension.real);
	t_c.imaginary = data->im_end - ((long double)y / WINDOW_HEIGHT * t_dimension.imaginary);
	return (t_c);
}

int	render(t_data *data)
{
	t_complex	t_c;
	int			color;

	if (data->win_ptr == NULL)
		return (1);
	if (!data->is_rendering)
		return (0);
	data->pixels_rendered_this_frame = 0; //Reset FLOP counter for this frame
	while (data->current_pixel_y < WINDOW_HEIGHT &&
		data->pixels_rendered_this_frame < MAX_CALCULATIONS_PER_FRAME)
		{
			while (data->current_pixel_x < WINDOW_WIDTH &&
				data->pixels_rendered_this_frame < MAX_CALCULATIONS_PER_FRAME)
			{
				t_c = ft_fractal_mapping(data->current_pixel_x, data->current_pixel_y, data);
				color = ft_fractal_color(t_c, data);
				img_pix_put(&data->img, data->current_pixel_x, data->current_pixel_y, color);
				data->current_pixel_x++;
			}
			if (data->current_pixel_x >= WINDOW_WIDTH)
			{
				data->current_pixel_x = 0;
				data->current_pixel_y++;
			}
		}
	if (data->current_pixel_y >= WINDOW_HEIGHT)
	{
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
		if (data->render_pass_max_iter < MAX_ITER)
		{
			data->render_pass_max_iter += 50;
			if (data->render_pass_max_iter > MAX_ITER)
				data->render_pass_max_iter = MAX_ITER;
			data->current_pixel_x = 0;
			data->current_pixel_y = 0;
			ft_printf("Render pass complete. Increasing MAX_ITER to %d.\n", data->render_pass_max_iter);
		}
		else
		{
			data->is_rendering = 0;
			ft_printf("Rendering complete to MAX_ITER %d.\n", MAX_ITER);
		}
	}
	else
	{
	
	}
	return (0);
}

int	handle_destroy_notify(t_data *data)
{
	if (data->win_ptr)
	{
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		data->win_ptr = NULL;
	}
	return (0);
}

int	handle_mouse_event(int button, int x, int y, t_data *data)
{
	t_complex	mouse_complex_pos;
	double		zoom_factor;

	if(button == SCROLL_UP)
		zoom_factor = ZOOM_IN_FACTOR;
	else if (button == SCROLL_DOWN)
		zoom_factor = ZOOM_OUT_FACTOR;
	else
		return (0);
	mouse_complex_pos = ft_fractal_mapping(x, y, data);
	data->re_start = mouse_complex_pos.real - (mouse_complex_pos.real - data->re_start) * zoom_factor;
	data->re_end = mouse_complex_pos.real + (data->re_end - mouse_complex_pos.real) * zoom_factor;
	data->im_start = mouse_complex_pos.imaginary - (mouse_complex_pos.imaginary - data->im_start) * zoom_factor;
	data->im_end = mouse_complex_pos.imaginary + (data->im_end - mouse_complex_pos.imaginary) * zoom_factor;
	data->current_pixel_x = 0;
	data->current_pixel_y = 0;
	data->render_pass_max_iter = 50;
	data->is_rendering = 1;
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
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "fract'ol");
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
	data.re_start = -2.0;
	data.re_end = 1.0;
	data.im_start = -1.5;
	data.im_end = 1.5;
	data.current_pixel_x = 0;
	data.current_pixel_y = 0;
	data.render_pass_max_iter = 50;
	data.is_rendering = 1;
	//mlx_loop_hook: for always-running logic, like animation or redrawing:
 	mlx_loop_hook(data.mlx_ptr, &render, &data);
	//mlx_hook: for specific events, like key press, mouse movement, window resize:
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);
	mlx_hook(data.win_ptr, 17, 0, &handle_destroy_notify, &data);
	mlx_hook(data.win_ptr, ButtonPress, ButtonPressMask, &handle_mouse_event, &data);
	/*mlx_loop: Starts the event main loop. It runs forever (until you manually destroy 
	 * the window or exit), waiting for user input (keyboard/mouse) or internal events, 
	 * and dispatches them to your hook functions (like mlx_hook() or mlx_loop_hook()).*/
	mlx_loop(data.mlx_ptr);
	/*exits the loop when there's no window left or escape key is pressed.*/
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
}

