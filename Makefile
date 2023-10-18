cc=GCC
CFLAGS= -std=c99 -pedantic -Wall -Wextra -Werror

scanner: scanner.c
	$(CC) $(CFLAGS) scanner.c -o scanner