.PHONY: clean

all:
	gcc main.c -std=c11 -o main `pkg-config openal --libs --cflags` -O2 -Wall -Wextra

clean:
	rm -f main
