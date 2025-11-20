NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Werror -Wextra
HEADER = ft_printf.h
SRC = ft_putstr_fd.c ft_putnbr_fd.c ft_putchar_fd.c \
	ft_strlen.c ft_printf.c ft_putunnbr_fd.c ft_puthex_fd.c ft_putptr_fd.c
OBJ = $(SRC:.c=.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
all: $(NAME)
$(NAME): $(OBJ)
	ar rcs $(NAME) $^
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
