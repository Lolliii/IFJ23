cc=GCC
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror

all: scanner.c expr_parser.c expr_stack.c main.c
	$(CC) $(CFLAGS) scanner.c expr_parser.c expr_stack.c main.c -o prog

clean:
	rm -f prog
