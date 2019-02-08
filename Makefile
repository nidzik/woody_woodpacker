
NAME := woody_woodpacker
CC := gcc -fno-stack-protector
# CFLAGS := -Werror
CPATH = src/
CFILES = \
	main.c \
	find_cave.c \
	inject_code.c

OPATH = obj/
OFILES = $(CFILES:.c=.o)
OBJ = $(addprefix $(OPATH), $(OFILES))
HPATH = inc/

HFILES = \
	inc/woody.h

INC = $(addprefix -I./, $(HPATH))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OPATH) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)%.o: $(CPATH)%.c $(HFILES)
	$(CC) $(INC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

it : re
	gcc test/test.c -o test_bin
	./$(NAME) test_bin
	./woody

