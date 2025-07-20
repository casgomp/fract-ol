#include "../include/fractol.h"

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
			img_pix_put(&data->img, data->current_pixel_x++, data->current_pixel_y, color);
		}
		if (data->current_pixel_x >= WINDOW_WIDTH)
		{
			data->current_pixel_x = 0;
			data->current_pixel_y++;
		}
	}
	return (ft_put_image(data));
}

int	ft_put_image(t_data *data)
{
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
			ft_printf("Current pixel at (%i, %i).\n", data->current_pixel_x, data->current_pixel_y);
		}
	}
	return (0);
}

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

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}
