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

#include <stdio.h>

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;


    // if(argc < 2){
    //     fprintf(stderr, "ERROR: Too few arguments.\n");
    //     return 1;
    // }
    
    // TODO:
    // zkontrolovat spravne otevreni souboru
    // FILE* file = fopen(argv[1], "r+");
    // FILE* file = fopen("stdin", "r+");
    FILE* file = stdin;
    //printf("NAZEV: %s\n\n", argv[1]);
    
    T_queue *queue = queue_init();
    
    //expr_parser(file, queue);

    T_token token = getNextToken(file);

// ______ parser
    prog(token, queue, file);


    free(queue);

    /*T_queue *queue = queue_init();
    T_token token = getNextToken(file);
    queue_add(queue, token);
    T_token token1 = getToken(queue, file);
    T_token token2 = getToken(queue, file);
    free(queue);*/
    
    /*T_token token;

    while ((token = getNextToken(file)).type != TOKEN_EOF) {
        if(token.type == TOKEN_ERROR){
            printf("chyba\n");
        } else {
            //printf("%d\n", token.type);
            printTokenName(token);
            printf("\n");
            if(token.valueLength)
                free(token.value);
        }
    }
    if(token.type == TOKEN_EOF){
        printf("KONEC\n");
    }
    if(token.valueLength)
        free(token.value);

    file = NULL;
*/
    // fclose(file);

    return 0;
}
