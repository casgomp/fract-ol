#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdio.h>
# include <stdlib.h>
# include <mlx.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <math.h>
# include "../lib/libft/ft_printf.h"

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 800
# define MLX_ERROR 1

# define MAX_ITER 1000
# define MAX_CALCULATIONS_PER_FRAME 10000000

# define SCROLL_UP 4
# define SCROLL_DOWN 5
# define ZOOM_IN_FACTOR 0.8
# define ZOOM_OUT_FACTOR 1.2

typedef struct	s_complex
{
	long double	real;
	long double	imaginary;
}	t_complex;

typedef struct	s_img
{
	void		*mlx_img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
}	t_img;

typedef struct	s_data
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	int			current_pixel_x;
	int			current_pixel_y;
	int			render_pass_max_iter;
	int			pixels_rendered_this_frame;
	int			is_rendering;
	int			fractal_type;
	t_complex	julia_c;
	long double	re_start;
	long double	re_end;
	long double	im_start;
	long double	im_end;

	int			*palette;
	int			palette_size;

}	t_data;

//fractol
int		main(int argc, char **argv);
int		ft_minilibx_setup(t_data *data);
void	ft_data_initialize(t_data *data);

//events
int			handle_destroy_notify(t_data *data);
int			handle_keypress(int keysym, t_data *data);
int			handle_mouse_event(int button, int x, int y, t_data *data);

//rendering
int			render(t_data *data);
int			ft_put_image(t_data *data);
int			ft_fractal_color(t_complex t_c, t_data *data); 
t_complex	ft_fractal_mapping(int x, int y, t_data *data);
void		img_pix_put(t_img *img, int x, int y, int color);

//rendering utils
int			ft_fractal_interpolate_color(int color1, int color2, long double t);
int			create_trgb(int t, int r, int g, int b);
void    	ft_generate_palette(t_data *data);

//fractal calculations
long double	ft_fractal_escape(t_complex t_c, t_data *data);
long double	ft_fractal_recursion(t_complex t_z, t_complex t_c, int count_iter, t_data *data);
long double	ft_fractal_smooth(int count_iter, t_complex t_z);
long double	ft_fractal_absolute_z(t_complex t_z);
t_complex	ft_fractal_new_z(t_complex t_z, t_complex t_c);

#endif
