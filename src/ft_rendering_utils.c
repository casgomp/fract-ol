#include "../include/fractol.h"

void    ft_generate_palette(t_data *data)
{
    int i;
    int r;
	int	g;
	int	b;

    data->palette_size = 512;
    data->palette = (int *)malloc(sizeof(int) * data->palette_size);
    if (!data->palette)
        return ;
    i = 0;
	while (i < data->palette_size)
    {
        r = (int)(sin(0.1 * i + 0) * 127 + 128);
        g = (int)(sin(0.1 * i + 2 * M_PI / 3) * 127 + 128);
        b = (int)(sin(0.1 * i + 4 * M_PI / 3) * 127 + 128);
        if (r < 0) r = 0; if (r > 255) r = 255;
        if (g < 0) g = 0; if (g > 255) g = 255;
        if (b < 0) b = 0; if (b > 255) b = 255;
        data->palette[i] = create_trgb(0, r, g, b);
		i ++;
    }
}

int create_trgb(int t, int r, int g, int b)
{
    return (t << 24 | r << 16 | g << 8 | b);
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
