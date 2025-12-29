NAME = push_swap
CC = cc
DEP = tqdm
CFLAGS = -Wall -Werror -Wextra
HELP_KIT = help_kit/
SRC = calculation_stacks.c do_smart_rot.c find_out_cheapeast.c ft_array.c \
      main.c ft_atoi.c ft_putstr.c helper_func.c operations.c sort.c sort_n.c

OBJ = $(SRC:.c=.o)

QUIET :=

ifeq ($(MAKECMDGOALS),test)
	QUIET := @
endif

all: $(NAME)

$(NAME): $(OBJ)
	$(QUIET)$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(QUIET)rm -f $(OBJ)

test: $(NAME) clean check-deps
	@ python3 $(HELP_KIT)test.py

fclean: clean
	$(QUIET)rm -f $(NAME)

re: fclean all

check-deps:
	@pip show $(DEP) >/dev/null 2>&1 || (pip3 install $(DEP))

.PRONY: check-deps run clean fclean test all
