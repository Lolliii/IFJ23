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

int main(int argc, char *argv[]) {
    
    if(argc < 2){
        fprintf(stderr, "ERROR: Too few arguments.\n");
        return 1;
    }

    printf("NAZEV: %s\n", argv[1]);

    // TODO:
    // zkontrolovat spravne otevreni souboru
    FILE* file = fopen("test.txt", "r+");
    
    T_token token;

    while ((token = getNextToken(file)).type != TOKEN_EOF) {
        if(token.type == TOKEN_ERROR){
            printf("chyba\n");
        } else {
            printf("%d\n", token.type);
        }
    }
    if(token.type == TOKEN_EOF){
        printf("KONEC\n");
    }

    fclose(file);
    file = NULL;

    return 0;
}
