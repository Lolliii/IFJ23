/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
#include "error.h"
#include "expr_stack.h"
#include "expr_parser.h"
#include "parser_queue.h"
#include "parser.h"
#include "symtable.h"

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    FILE* file = stdin;    
    T_queue *queue = queue_init();
    
    //expr_parser(file, queue);

        T_token token = getNextToken(file);
// ______ parser
        prog(token, queue, file);

    free(queue);

    return 0;
}
