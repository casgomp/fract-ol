#include "../include/fractol.h"

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
