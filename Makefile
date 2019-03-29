NAME := woody_woodpacker
CC := gcc -g
ASM := nasm -f elf64 
# CFLAGS := -Werror
CPATH = src/
CFILES = \
	main.c \
	find_cave.c \
	inject_code.c \
	inject_code_pe.c \
	section.c \
	section_pe.c \
	segments.c \
	encrypt.s \
	key.c \
	utils.c

OPATH = obj/
OFILES = $(addsuffix .o, $(CFILES))
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

$(OPATH)%.c.o: $(CPATH)%.c $(HFILES)
	$(CC) $(INC) $(CFLAGS) -c -o $@ $<

$(OPATH)%.s.o: $(CPATH)%.s
	$(ASM) -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

it : re
	gcc test/test.c -o test_bin
	./$(NAME) test_bin
	./woody
