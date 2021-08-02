all: main.c
	gcc main.c -o inc

run: all
	./inc

leak: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./inc test1.in

