cc=GCC
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror -g

all: scanner.c expr_parser.c expr_stack.c parser_queue.c parser.c main.c symtable.c
	$(CC) $(CFLAGS) scanner.c expr_parser.c expr_stack.c parser_queue.c parser.c main.c symtable.c -o prog

clean:
	rm -f prog
