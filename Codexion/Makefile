NAME = codexion
CC = cc
CFLAGS = -Wall -Werror -Wextra -g -pthread
SRC = coders.c dongle.c helper_func.c scheduler.c start.c \
	  conv_arg.c ft_array.c main.c stages.c ft_array_expanded.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all
