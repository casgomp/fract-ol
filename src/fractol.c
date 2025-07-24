/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:11:15 by pecastro          #+#    #+#             */
/*   Updated: 2025/07/24 10:33:12 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/fractol.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		pixel_x;
	int		pixel_y;

	ft_data_initialize(&data);
	if (argc < 2)
		return (ft_printf("1=Mandelbrot 2=Julia\n"), -1);
	if (ft_atoi(argv[1]) == 1)
		data.fractal_type = 0;
	else if (ft_atoi(argv[1]) == 2)
	{
		if (argc < 4)
			return (ft_printf("Julia set? (x, y) for c as u int\n"), -1);
		data.fractal_type = 1;
		pixel_x = ft_atoi(argv[2]);
		pixel_y = ft_atoi(argv[3]);
		if (pixel_x < 0 || pixel_x >= WIN_W || pixel_y < 0 || 
			pixel_y >= WIN_H)
			return (ft_printf(">= 0 ; x < %i: y < %i\n",
					WIN_W, WIN_H), -1);
		data.julia_c = ft_fractal_mapping(pixel_x, pixel_y, &data);
	}
	else
		return (ft_printf("1=Mandelbrot 2=Julia\n"), -1);
	return (ft_minilibx_setup(&data), 0);
}

void	ft_data_initialize(t_data *data)
{
	data->re_start = -2.0;
	data->re_end = 1.0;
	data->im_start = -1.5;
	data->im_end = 1.5;
	data->current_pixel_x = 0;
	data->current_pixel_y = 0;
	data->render_pass_max_iter = 50;
	data->is_rendering = 1;
}

int	ft_minilibx_setup(t_data *data)
{
	data->mlx_ptr = mlx_init();
	if (data->mlx_ptr == NULL)
		return (MLX_ERROR);
	data->win_ptr = mlx_new_window(data->mlx_ptr,
			WIN_W, WIN_H, "fract'ol");
	if (data->win_ptr == NULL)
		return (free(data->win_ptr), MLX_ERROR);
	data->img.mlx_img = mlx_new_image(data->mlx_ptr, WIN_W, WIN_H);
	data->img.addr = mlx_get_data_addr(data->img.mlx_img, &data->img.bpp,
			&data->img.line_len, &data->img.endian);
	ft_generate_palette(data);
	mlx_loop_hook(data->mlx_ptr, &render, data);
	mlx_hook(data->win_ptr, 17, 0, &handle_destroy_notify, data);
	mlx_hook(data->win_ptr, KeyPress, KeyPressMask, &handle_keypress, data);
	mlx_hook(data->win_ptr, ButtonPress, ButtonPressMask,
		&handle_mouse_event, data);
	mlx_loop(data->mlx_ptr);
	mlx_destroy_image(data->mlx_ptr, data->img.mlx_img);
	mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	if (data->palette)
		free(data->palette);
	return (0);
}
/*Setup hooks (i.e., callbacks: functions to be executed later,
 * when certain events occur inside the loop):*/
//mlx_new_image returns the address of the image stored in 
//shared memory(mlx_img):
//mlx_get_data_addr returns a char*. The bpp,line_len and endian 
//are modified 
//by the function itself (bpp is usually 32, because each pixel has 4 bytes.
//	line_len (if there's no padding) is the number of bytes per row, i.e., 
//	image_width * (bpp / 8), and endian depends on the machine 
//	(42 machines are LE) 
//mlx_loop_hook: for always-running logic, like animation or redrawing:
//mlx_hook: for specific events, like key press, mouse movement, window resize:
/*mlx_loop: Starts the event main loop. It runs forever 
 * (until you manually destroy 
 * the window or exit), waiting for user input (keyboard/mouse) or 
 * internal events, and dispatches them to your hook functions 
 * (like mlx_hook() or mlx_loop_hook()).*/
/*exits the loop when there's no window left or escape key is pressed.*/
