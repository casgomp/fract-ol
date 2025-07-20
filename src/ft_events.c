#include "../include/fractol.h"

int	handle_destroy_notify(t_data *data)
{
	if (data->win_ptr)
	{
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		data->win_ptr = NULL;
	}
	return (0);
}

int	handle_keypress(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
	{
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		data->win_ptr = NULL;
	}
	else if (keysym == XK_space)
	{
		t_complex c = ft_fractal_mapping(data->current_pixel_x, data->current_pixel_y, data); 
		ft_printf("Pixel: (%d, %d)\n", data->current_pixel_x, data->current_pixel_y);
		printf("Julia seed: (%.6Lf + %Lfi\n", c.real, c.imaginary);
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
