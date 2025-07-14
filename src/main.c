#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>
#include <X11/keysym.h>
#include <X11/X.h>

# define WINDOW_WIDTH 600
# define WINDOW_HEIGHT 300
# define MLX_ERROR 1
# define RED_PIXEL 0xFF0000
# define GREEN_PIXEL 0x00FF00
# define WHITE_PIXEL 0xFFFFFF

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
}	t_data;

typedef struct	s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rect;

/*These function prototypes are already in the mlx 
 * header files,so no need to include them:
 * void	*mlx_new_image(void *mlx_ptr, int width, int height);
 * char	*mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);
 * int	mlx_put_image_to_window(t_xvar *xvar,t_win_list *win,t_img *img, int x,int y);*/

/* Use this function(renders image first and then pushes it to the window) 
 * instead of mlx_pixel_put (draws pixel on window directly without any 
 * delay causing a flickering effect)*/
void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	//locate a pixel (addr is base, i.e., 0,0; y * img->line_len skips down y rows
	//and x * (bpp / 8) skips x pixels into the row):
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	//color the pixel:
	*(int *)pixel = color;
}

int	render_rect(t_img *img, t_rect rect)
{
	int	i;
	int	j;

	i = rect.y;
	j = 0;
	while (i < rect.y + rect.height)
	{
		j = rect.x;
		while (j < rect.x + rect.width)
			img_pix_put(img, j++, i, rect.color);
		i ++;
	}
	return (0);
}

void	render_background(t_img *img, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < WINDOW_HEIGHT)
	{
		j = 0;
		while (j < WINDOW_WIDTH)
			img_pix_put(img, j++, i, color);
		i ++;
	}
}

int	render(t_data *data)
{
	if (data->win_ptr == NULL)
		return (1);
	render_background(&data->img, WHITE_PIXEL);
	render_rect(&data->img, (t_rect){WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100, 100, 100, GREEN_PIXEL});
	render_rect(&data->img, (t_rect){0, 0, 100, 100, RED_PIXEL});
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);
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

int	main(void)
{
	t_data	data;

	data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (MLX_ERROR);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "My first window!");
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
	//mlx_loop_hook: for always-running logic, like animation or redrawing:
 	mlx_loop_hook(data.mlx_ptr, &render, &data);
	//mlx_hook: for specific events, like key press, mouse movement, window resize:
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);
	
	/*mlx_loop: Starts the event main loop. It runs forever (until you manually destroy 
	 * the window or exit), waiting for user input (keyboard/mouse) or internal events, 
	 * and dispatches them to your hook functions (like mlx_hook() or mlx_loop_hook()).*/
	mlx_loop(data.mlx_ptr);

	/*exits the loop when there's no window left or escape key is pressed.*/
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
}

