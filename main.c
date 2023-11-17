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

#include <stdio.h>

int main(int argc, char *argv[]) {
    
    if(argc < 2){
        fprintf(stderr, "ERROR: Too few arguments.\n");
        return 1;
    }
    
    // TODO:
    // zkontrolovat spravne otevreni souboru
    FILE* file = fopen("test.txt", "r+");
    printf("NAZEV: %s\n", argv[1]);
    
    //expr_parser(file);
    
    T_token token;

    while ((token = getNextToken(file)).type != TOKEN_EOF) {
        if(token.type == TOKEN_ERROR){
            printf("chyba\n");
        } else {
            //printf("%d\n", token.type);
            printTokenName(token);
            printf("\n");
        }
    }
    if(token.type == TOKEN_EOF){
        printf("KONEC\n");
    }

    fclose(file);
    file = NULL;

    return 0;
}