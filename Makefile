CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilib/libft/include -Iminilibx-linux
MLX_FLAGS = -Lminilibx-linux -lmlx -lX11 -lXext -lm
MLX_LIB = minilibx-linux/libmlx.a

NAME = fractol

OBJ_DIR = objs

S_PRINTF	= ft_printf.c ft_putchar_prnt.c ft_putnbr_base_prnt.c \
			  ft_putstr_prnt.c ft_putnbr_base_prnt_address.c \
			  ft_putnbr_base_prnt_address_pre.c
			  
S_STRING	= ft_strlen.c ft_atoi.c

S_FRACTOL	= fractol.c ft_events.c ft_fractal_calculations.c \
			  ft_rendering.c ft_rendering_utils.c

SRCS = $(S_PRINTF) $(S_STRING) $(S_FRACTOL)
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

vpath %.c src lib/libft/src/printf lib/libft/src/string

all: $(NAME)

$(NAME): $(MLX_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(MLX_LIB):
	@make -C minilibx-linux

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f minilibx-linux/libmlx.a

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re




