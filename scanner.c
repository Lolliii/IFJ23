/*
IFJ Projekt 2023

SCANNER - Lexical analysis
Gets called by parser by get_tokens. Scanner gets a reference to empty token in parameter,
fills it and returns NO_ERROR. In case of an error, the return value is LEX_ERROR

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
// #include <unistd.h>

/*
Priprav soubor jako statickou globalni promennou - kvuli tomu ze by se jinak musel passovat i soubor,
a byl by problem s tim kde se skoncilo. Takhle si to bude pamatovat. A parser to bude delat po jednom 
tokenu, ne ze se to natokenizuje cele najednou (kvuli pameti pri obrovskych souborech)
*/

//static FILE* file = NULL;

int main(int argc, char *argv[]) {
    
    if(argc < 2){
        fprintf(stderr, "ERROR: Too few arguments.\n");
        return 1;
    }

    printf("NAZEV: %s\n", argv[1]);

    //FILE* file = stdin;
    FILE* file = fopen("test.txt", "r+");
    
    T_token token;

    while ((token = getNextToken(file)).type != TOKEN_EOF) {
        if(token.type == TOKEN_ERROR){
            printf("chyba\n");
        } else {
            printf("%d\n", token.type);
        }
    }

    fclose(file);
    return 0;
}
