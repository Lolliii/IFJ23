cc=GCC
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror -g

all: scanner.c expr_parser.c expr_stack.c parser_queue.c parser.c main.c symtable.c code_gen.c
	$(CC) $(CFLAGS) scanner.c expr_parser.c expr_stack.c parser_queue.c parser.c main.c symtable.c code_gen.c -o prog

clean:
	rm -f prog
