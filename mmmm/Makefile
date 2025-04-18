CFLAGS = -Wall -Wextra -Werror
CC = cc
NAME = libftprintf.a
SRC =ft_printf.c ft_Prints_single_character.c \
	ft_Prints_hexadecimal.c ft_Prints_unsigned_decimal.c \
	ft_Prints_string.c ft_Prints_decimal.c ft_prints_pointer.c


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		ar rc $(NAME) $(OBJ)

%.o: %.c  ft_printf.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
