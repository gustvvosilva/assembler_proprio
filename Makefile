all:
	gcc main.c tokens.c -o main

run: all
	./main

clean:
	rm -f main
	rm -f memoria.mem

neander: run
	./neander memoria.mem
