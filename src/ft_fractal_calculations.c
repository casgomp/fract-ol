#include "../include/fractol.h"	

long double	ft_fractal_escape(t_complex t_c, t_data *data)
{
	t_complex	t_z;

	if (data->fractal_type == 0)
	{
		t_z.real = 0;
		t_z.imaginary = 0;
		return (ft_fractal_recursion(t_z, t_c, 0, data));
	}
	else
	{
		t_z = t_c;
		return (ft_fractal_recursion(t_z, data->julia_c, 0, data));
	}
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

long double	ft_fractal_absolute_z(t_complex t_z)
{
	long double	absolute_z;

	absolute_z = (t_z.real * t_z.real) + (t_z.imaginary * t_z.imaginary);
	return (absolute_z);
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
