all: main.c
	@gcc -std=c11 -O2 -Wall -Wextra -o main main.c -lgmp