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

void printTokenName(T_token token) {
    switch (token.type) {
        case TOKEN_ERROR:
            printf("TOKEN_ERROR");
            break;
        case TOKEN_EOF:
            printf("TOKEN_EOF");
            break;
        case TOKEN_COMMA:
            printf("TOKEN_COMMA");
            break;
        case TOKEN_COLON:
            printf("TOKEN_COLON");
            break;
        case TOKEN_PLUS:
            printf("TOKEN_PLUS");
            break;
        case TOKEN_MINUS:
            printf("TOKEN_MINUS");
            break;
        case TOKEN_FUNCTION_TYPE:
            printf("TOKEN_FUNCTION_TYPE");
            break;
        case TOKEN_MUL:
            printf("TOKEN_MUL");
            break;
        case TOKEN_QUESTION_MARK:
            printf("TOKEN_QUESTION_MARK");
            break;
        case TOKEN_DOUBLE_QUESTION_MARK:
            printf("TOKEN_DOUBLE_QUESTION_MARK");
            break;
        case TOKEN_UNDERSCORE:
            printf("TOKEN_UNDERSCORE");
            break;
        case TOKEN_ID:
            printf("TOKEN_ID");
            break;
        case TOKEN_TYPE_ID:
            printf("TOKEN_TYPE_ID");
            break;
        case TOKEN_EXCLAMATION_MARK:
            printf("TOKEN_EXCLAMATION_MARK");
            break;
        case TOKEN_NOT_EQUAL:
            printf("TOKEN_NOT_EQUAL");
            break;
        case TOKEN_ASSIGN:
            printf("TOKEN_ASSIGN");
            break;
        case TOKEN_EQUAL:
            printf("TOKEN_EQUAL");
            break;
        case TOKEN_GT:
            printf("TOKEN_GT");
            break;
        case TOKEN_GTE:
            printf("TOKEN_GTE");
            break;
        case TOKEN_LT:
            printf("TOKEN_LT");
            break;
        case TOKEN_LTE:
            printf("TOKEN_LTE");
            break;
        case TOKEN_L_CURL:
            printf("TOKEN_L_CURL");
            break;
        case TOKEN_R_CURL:
            printf("TOKEN_R_CURL");
            break;
        case TOKEN_L_SQR:
            printf("TOKEN_L_SQR");
            break;
        case TOKEN_R_SQR:
            printf("TOKEN_R_SQR");
            break;
        case TOKEN_R_RND:
            printf("TOKEN_R_RND");
            break;
        case TOKEN_L_RND:
            printf("TOKEN_L_RND");
            break;
        case TOKEN_SINGLE_QUOTE:
            printf("TOKEN_SINGLE_QUOTE");
            break;
        case TOKEN_SLASH:
            printf("TOKEN_SLASH");
            break;
        case TOKEN_BSLASH:
            printf("TOKEN_BSLASH");
            break;
        case TOKEN_STRING:
            printf("TOKEN_STRING");
            break;
        case TOKEN_ML_STRING:
            printf("TOKEN_ML_STRING, delka: %d", token.valueLength);
            break;
        case TOKEN_INT:
            printf("TOKEN_INT");
            break;
        case TOKEN_INT_EXP:
            printf("TOKEN_INT_EXP");
            break;
        case TOKEN_INT_EXP_PM:
            printf("TOKEN_INT_EXP_PM");
            break;
        case TOKEN_DOUBLE:
            printf("TOKEN_DOUBLE");
            break;
        case TOKEN_DOUBLE_EXP:
            printf("TOKEN_DOUBLE_EXP");
            break;
        case TOKEN_DOUBLE_EXP_PM:
            printf("TOKEN_DOUBLE_EXP_PM");
            break;
        case TOKEN_LINE_FEED:
            printf("TOKEN_LINE_FEED");
            break;
        default:
            printf("Unknown Token Type");
            break;
    }
    if(token.valueLength){
        printf("->%s", token.value);
        // free(token.value);
    }

}

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
            printf("chyb\n");
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
