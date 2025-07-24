/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rendering.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:12:37 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 10:45:51 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/fractol.h"

int	render(t_data *data)
{
	t_complex	t_c;

	if (data->win_ptr == NULL)
		return (1);
	if (!data->is_rendering)
		return (0);
	data->pixels_rendered_this_frame = 0;
	while (data->current_pixel_y < WIN_H
		&& data->pixels_rendered_this_frame < MAX_CALCULATIONS_PER_FRAME)
	{
		while (data->current_pixel_x < WIN_W
			&& data->pixels_rendered_this_frame < MAX_CALCULATIONS_PER_FRAME)
		{
			t_c = ft_fractal_mapping(data->current_pixel_x,
					data->current_pixel_y, data);
			img_pix_put(&data->img, data->current_pixel_x++,
				data->current_pixel_y, ft_fractal_color(t_c, data));
		}
		if (data->current_pixel_x >= WIN_W)
		{
			data->current_pixel_x = 0;
			data->current_pixel_y++;
		}
	}
	return (ft_put_image(data));
}

int	ft_put_image(t_data *data)
{
	if (data->current_pixel_y >= WIN_H)
	{
		mlx_put_image_to_window(data->mlx_ptr,
			data->win_ptr, data->img.mlx_img, 0, 0);
		if (data->render_pass_max_iter < MAX_ITER)
		{
			data->render_pass_max_iter += 50;
			if (data->render_pass_max_iter > MAX_ITER)
				data->render_pass_max_iter = MAX_ITER;
			data->current_pixel_x = 0;
			data->current_pixel_y = 0;
			ft_printf("Render pass complete."
				"Increasing MAX_ITER to %d.\n", data->render_pass_max_iter);
		}
		else
		{
			data->is_rendering = 0;
			ft_printf("Rendering complete to MAX_ITER %d.\n", MAX_ITER);
			ft_printf("Current pixel at (%i, %i).\n",
				data->current_pixel_x, data->current_pixel_y);
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
	t_c.real = data->re_start + ((long double)x / WIN_W * t_dimension.real);
	t_c.imaginary = data->im_end
		- ((long double)y / WIN_H * t_dimension.imaginary);
	return (t_c);
}

int	ft_fractal_color(t_complex t_c, t_data *data)
{
	long double	count_iter_smooth;
	long double	scaled_iter;
	int			color;
	int			idx1;
	int			idx2;

	count_iter_smooth = ft_fractal_escape(t_c, data);
	if (count_iter_smooth == (long double)data->render_pass_max_iter)
		color = 0x000000;
	else
	{
		scaled_iter = count_iter_smooth * 10.0;
		scaled_iter = fmodl(scaled_iter, (long double)data->palette_size);
		idx1 = (int)floorl(scaled_iter);
		idx2 = (idx1 + 1) % data->palette_size;
		color = ft_fractal_interpolate_color(data->palette[idx1],
				data->palette[idx2], scaled_iter - (long double)idx1);
	}
	return (color);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}
