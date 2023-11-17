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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h> // kvuli uint_32

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

int check_keywords(char *check){
    //strcmp return values -> 0 = equal, 1 = first doesnt match, 2 = second doesnt match
    if((strcmp(check, "else")) == 0) return 1;
    else if((strcmp(check, "float")) == 0) return 2;
    else if((strcmp(check, "function")) == 0) return 1;
    else if((strcmp(check, "if")) == 0) return 1;
    else if((strcmp(check, "int")) == 0) return 3;
    else if((strcmp(check, "null")) == 0) return 1;
    else if((strcmp(check, "return")) == 0) return 1;
    else if((strcmp(check, "string")) == 0) return 4;
    else if((strcmp(check, "void")) == 0) return 1;
    else if((strcmp(check, "while")) == 0) return 1;
    else return 0;
}

void return_back(char c, FILE* file)
{
    if(c == EOF)
        fseek(file, 0, SEEK_CUR);
    else
        fseek(file, -1, SEEK_CUR);
}

// TOHLE taky jeste zkontroluju
// Function prints an error according to given error code 
void error_caller(int error_code){
    printf("\n");
    switch(error_code){
        case LEX_ERROR:
            printf("Error: %d -> Wrong lexem structure on line %d.\n", error_code, __LINE__);
            break;
        case SYN_ERROR:
            printf("Error: %d -> Wrong syntax on line %d.\n", error_code, __LINE__);
            break;
        case UNDEF_FUNCTION_ERROR:
            printf("Error: %d -> Undefined function or redefining attempt on line %d.\n", error_code, __LINE__);
            break;
        case PARAM_ERROR:
            printf("Error: %d -> Wrong type, number of parameters or return type on line %d.\n", error_code, __LINE__);
            break;
        case UNDEF_UNINIT_VARIABLE_ERROR:
            printf("Error: %d -> Using undefined variable on line %d.\n", error_code, __LINE__);
            break;
        case EXPRESSION_ERROR:
            printf("Error: %d -> Too little or too many expressions in return command on line %d.\n", error_code, __LINE__);
            break;
        case TYPE_COMP_ERROR:
            printf("Error: %d -> Wrong type in aritmetics, string, and relation expressions on line %d.\n", error_code, __LINE__);
            break;
        case OTHER_ERROR:
            printf("Error: %d -> Other semantic error on line %d.\n", error_code, __LINE__);
            break;
        case INTER_ERROR:
            printf("Error: %d -> Compilator error (allocation error, ...).\n", error_code);
            break;
    }
}

// TODO =============
// Dynamicky string + alokace
// ??? Zkontrolovat u ID klicova slova

/*
Funkce vraci token.
V pripade chyby vraci TOKEN_ERROR, a bude to muset byt osetreno volajicim jako LEX_ERROR
file je pripraveno v scanner.c
*/
T_token getNextToken(FILE* file){

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

    // EOF - Konec souboru
    token.type = TOKEN_EOF;
    return token;
}