all:
	gcc main.c -o main

run: all
	./main

clean:
	rm -f main
