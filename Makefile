CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude -Ilib/libft/include
MLX_FLAGS = -lmlx -lX11 -lXext -lm
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

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re




