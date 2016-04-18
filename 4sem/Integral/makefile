all: integral.o calc.c
	gcc -Wall -Werror -O0 -std=c99 -pthread -o calc integral.o calc.c
integral.o: integral.c
	gcc -Wall -Werror -O0 -std=c99 -pthread -c -o integral.o integral.c
