CC = cc
CFLAGS = -Wall -Wextra -Werror
BUFFER_SIZE ?= 42
CPPFLAGS += -D BUFFER_SIZE=$(BUFFER_SIZE)
NAME = gnl_example
BONUS_NAME = gnl_bonus_example
SRCS = main.c get_next_line.c get_next_line_utils.c
BONUS_SRCS = main.c get_next_line_bonus.c get_next_line_utils_bonus.c

all: $(NAME)

$(NAME): $(SRCS) get_next_line.h Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRCS) $(LDFLAGS) $(LDLIBS) -o $@

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_SRCS) get_next_line_bonus.h Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -D GNL_BONUS $(BONUS_SRCS) $(LDFLAGS) $(LDLIBS) -o $@

run: $(NAME)
	./$(NAME) $(ARGS)

clean:
	$(RM) $(NAME) $(BONUS_NAME)

fclean: clean

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all bonus run clean fclean re

PYTHON ?= python3

test:
	CC="$(CC)" TEST_CFLAGS="$(CFLAGS)" $(PYTHON) -m unittest discover -s tests -v

.PHONY: test
