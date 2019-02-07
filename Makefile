
NAME := woody_woodpacker
CC := gcc
# CFLAGS := -Werror
CPATH = src/
CFILES = \
	main.c

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
	gcc src/test.c -o test 
	./$(NAME) test
	./woody

