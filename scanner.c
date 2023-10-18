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
#include <unistd.h>

/*
Priprav soubor jako statickou globalni promennou - kvuli tomu ze by se jinak musel passovat i soubor,
a byl by problem s tim kde se skoncilo. Takhle si to bude pamatovat. A parser to bude delat po jednom 
tokenu, ne ze se to natokenizuje cele najednou (kvuli pameti pri obrovskych souborech)
*/

//static FILE* file = NULL;

int main() {
    FILE* file = stdin;
    T_token token;

    // ty tokeny si pak stejne bude volat parser, tak jen pro testování jsem dal, aby se načetlo prvních 10 tokenů
    for(int i = 0; i < 10; i++)
    {
        token = get_token(file);
        printf("%d\n", token.type);
    }
    //while ((token = getNextToken()).type != EOF) {
          
    //}

    return 0;
}