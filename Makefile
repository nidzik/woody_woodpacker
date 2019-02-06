
all: op

op:
	@gcc main.c -o woody
clean:
	@rm -rf *.o
fclean : clean
	@rm -rf woody
re: fclean all


it : re
	gcc hello.c -o hello 
	cp hello hello_base
	./woody hello
	./hello 

