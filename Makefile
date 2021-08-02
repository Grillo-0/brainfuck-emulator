all: main.c
	gcc main.c -o inc

clean:
	rm *.o *.out inc

run: all
	./inc

leak: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./inc test1.in

