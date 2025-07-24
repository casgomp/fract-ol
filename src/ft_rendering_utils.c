/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rendering_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:12:46 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 11:49:36 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/fractol.h"

void	ft_generate_palette(t_data *data)
{
	int	i;
	int	r;
	int	g;
	int	b;

	data->palette_size = 512;
	data->palette = (int *)malloc(sizeof(int) * data->palette_size);
	if (!data->palette)
		return ;
	i = 0;
	while (i < data->palette_size)
	{
		r = rgb_cap((int)(sin(0.1 * i + 0) * 127 + 128));
		g = rgb_cap((int)(sin(0.1 * i + 2 * M_PI / 3) * 127 + 128));
		b = rgb_cap((int)(sin(0.1 * i + 4 * M_PI / 3) * 127 + 128));
		data->palette[i] = (r << 16 | g << 8 | b);
		i ++;
	}
}

int	rgb_cap(int color)
{
	if (color < 0)
		return (0);
	else if (color > 255)
		return (255);
	else
		return (color);
}

int	ft_fractal_interpolate_color(int color1, int color2, long double t)
{
	int	r;
	int	g;
	int	b;

	r = (int)(c_sep(color1, 1) + t * (c_sep(color2, 1) - c_sep(color1, 1)));
	g = (int)(c_sep(color1, 2) + t * (c_sep(color2, 2) - c_sep(color1, 2)));
	b = (int)(c_sep(color1, 3) + t * (c_sep(color2, 3) - c_sep(color1, 3)));
	return ((r << 16) | (g << 8) | b);
}

int	c_sep(int color, int rgb)
{
	int	r;
	int	g;
	int	b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	if (rgb == 1)
		return (r);
	else if (rgb == 2)
		return (g);
	else
		return (b);
}
