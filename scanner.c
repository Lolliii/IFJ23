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
        // case TOKEN_TYPE_ID:
        //     printf("TOKEN_TYPE_ID");
        //     break;
        // case TOKEN_TYPE_ID:
        //     printf("TOKEN_TYPE_ID");
        //     break;
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
            printf("TOKEN_ML_STRING");
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
        case TOKEN_KW_DOUBLE:
            printf("TOKEN_KW_DOUBLE");
            break;
        case TOKEN_KW_ELSE:
            printf("TOKEN_KW_ELSE");
            break;
        case TOKEN_KW_FUNC:
            printf("TOKEN_KW_FUNC");
            break;
        case TOKEN_KW_IF:
            printf("TOKEN_KW_IF");
            break;
        case TOKEN_KW_INT:
            printf("TOKEN_KW_INT");
            break;
        case TOKEN_KW_LET:
            printf("TOKEN_KW_LET");
            break;
        case TOKEN_KW_NIL:
            printf("TOKEN_KW_NIL");
            break;
        case TOKEN_KW_RETURN:
            printf("TOKEN_KW_RETURN");
            break;
        case TOKEN_KW_STRING:
            printf("TOKEN_KW_STRING");
            break;
        case TOKEN_KW_VAR:
            printf("TOKEN_KW_VAR");
            break;
        case TOKEN_KW_WHILE:
            printf("TOKEN_KW_WHILE");
            break;
        case TOKEN_TYPE_INT:
            printf("TOKEN_TYPE_INT");
            break;
        case TOKEN_TYPE_FLOAT:
            printf("TOKEN_TYPED_FLOAT");
            break;
        case TOKEN_TYPE_STRING:
            printf("TOKEN_TYPE_STRING");
            break;
        case TOKEN_KW_DOUBLE:
            printf("TOKEN_KW_DOUBLE");
            break;
        case TOKEN_KW_ELSE:
            printf("TOKEN_KW_ELSE");
            break;
        case TOKEN_KW_FUNC:
            printf("TOKEN_KW_FUNC");
            break;
        case TOKEN_KW_IF:
            printf("TOKEN_KW_IF");
            break;
        case TOKEN_KW_INT:
            printf("TOKEN_KW_INT");
            break;
        case TOKEN_KW_LET:
            printf("TOKEN_KW_LET");
            break;
        case TOKEN_KW_NIL:
            printf("TOKEN_KW_NIL");
            break;
        case TOKEN_KW_RETURN:
            printf("TOKEN_KW_RETURN");
            break;
        case TOKEN_KW_STRING:
            printf("TOKEN_KW_STRING");
            break;
        case TOKEN_KW_VAR:
            printf("TOKEN_KW_VAR");
            break;
        case TOKEN_KW_WHILE:
            printf("TOKEN_KW_WHILE");
            break;
        case TOKEN_TYPE_INT:
            printf("TOKEN_TYPE_INT");
            break;
        case TOKEN_TYPE_FLOAT:
            printf("TOKEN_TYPED_FLOAT");
            break;
        case TOKEN_TYPE_STRING:
            printf("TOKEN_TYPE_STRING");
            break;
        default:
            printf("Unknown Token Type");
            break;
    }
    if(token.valueLength)
        printf("->%s", token.value);
}

/*
Checks if passed param is one of {Double, else, func, if, Int, let, nil, return, String, var, while}
!! Is case-sensitive !!
Return values:
    0:  No match (not a keyword)
    1:  Double
    2:  else 
    3:  func
    4:  if
    5:  Int
    6:  let
    7:  nil
    8:  return
    9:  String
    10: var
    11: while
*/
/*
Checks if passed param is one of {Double, else, func, if, Int, let, nil, return, String, var, while}
!! Is case-sensitive !!
Return values:
    0:  No match (not a keyword)
    1:  Double
    2:  else 
    3:  func
    4:  if
    5:  Int
    6:  let
    7:  nil
    8:  return
    9:  String
    10: var
    11: while
*/
int check_keywords(char *check){
    if((strcmp(check, "Double")) == 0)  return 1;
    else if((strcmp(check, "else")) == 0) return 2;
    else if((strcmp(check, "func")) == 0) return 3;
    else if((strcmp(check, "if")) == 0) return 4;
    else if((strcmp(check, "Int")) == 0) return 5;
    else if((strcmp(check, "let")) == 0) return 6;
    else if((strcmp(check, "nil")) == 0) return 7;
    else if((strcmp(check, "return")) == 0) return 8;
    else if((strcmp(check, "String")) == 0) return 9;
    else if((strcmp(check, "var")) == 0) return 10;
    else if((strcmp(check, "while")) == 0) return 11;
    else return 0;
}


/*
Checks if passed param is one of {Double?, Int?, String?}
!! case-sensitive !!
Return values:
    0: No match
    1: Double?
    2: Int?
    3: String?
*/
int check_id_types(char *check){
    if((strcmp(check, "Double?")) == 0)  return 1;
    else if((strcmp(check, "Int?")) == 0) return 2;
    else if((strcmp(check, "String?")) == 0) return 3;
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

/*
Funkce vraci token.
V pripade chyby vraci TOKEN_ERROR, a bude to muset byt osetreno volajicim jako LEX_ERROR
file je pripraveno v scanner.c
*/
T_token getNextToken(FILE* file){

    T_token token;
    token.value = "\0";
    token.valueLength = 0;

    // Pomocne promenne pro naplnenni tokenu
    char value[1024] = "\0";                             // Tady to upravit na promennou delku nejak
    uint32_t length = 0;
    int hexLength = 0;
    int blockComms = 0;

    // Loop pro ziskani tokenu
    char c = fgetc(file);
    int state = S_START;
    while(c != EOF){
        if(state != S_START)
           c = fgetc(file);         // pokud znak nedostane po prvnim projiti finalni stav, je potreba nacist novy
    
        switch(state){
            case(S_START): 
            // Tady se to rozdeli podle toho jestli je to konecny stav nebo ne
            // Pokud z toho to stavu nelze uz nikam prejit, naplni a returnne se token

                    if(isspace(c)){         // whitespaces - bere i EOL
                        c = fgetc(file);     // při whitespace se to seklo v endless loopu
                        break;
                    } 
                    else if(c == ','){
                        token.type = TOKEN_COMMA;
                        return token;
                    } 
                    else if(c == ':'){
                        token.type = TOKEN_COLON;
                        return token;
                    } 
                    else if(c == '+'){
                        token.type = TOKEN_PLUS;
                        return token;
                    } 
                    else if(c == '-'){
                        state = S_MINUS;
                        break;
                    } 
                    else if(c == '*'){
                        token.type = TOKEN_MUL;
                        return token;
                    } 
                    else if(c == ':'){
                        token.type = TOKEN_COLON;
                        return token;
                    } 
                    else if(c == '?'){
                        state = S_QUESTION_MARK;
                        break;
                    } 
                    else if(c == '_'){
                        state = S_UNDERSCORE;
                        break;
                    } 
                    else if(isalpha(c)){
                        state = S_ID;
                        value[length] = c;
                        length++;
                        break;
                    } 
                    else if(c == '!'){
                        state = S_EXCLAMATION_MARK;
                        break;
                    } 
                    else if(c == '='){
                        state = S_ASSIGN;
                        break;
                    } 
                    else if(c == '>'){
                        state = S_GT;
                        break;
                    } 
                    else if(c == '<'){
                        state = S_LT;
                        break;
                    } 
                    else if(c == '}'){
                        token.type = TOKEN_R_CURL;
                        return token;
                    } 
                    else if(c == '{'){
                        token.type = TOKEN_L_CURL;
                        return token;
                    } 
                    else if(c == ']'){
                        token.type = TOKEN_R_SQR;
                        return token;
                    } 
                    else if(c == '['){
                        token.type = TOKEN_L_SQR;
                        return token;
                    } 
                    else if(c == ')'){
                        token.type = TOKEN_R_RND;
                        return token;
                    } 
                    else if(c == '('){
                        token.type = TOKEN_L_RND;
                        return token;
                    } 
                    else if(c == 39){        // Uvozovka ASCII
                        token.type = TOKEN_SINGLE_QUOTE;
                        return token;
                    } 
                    else if(c == '/'){
                        state = S_SLASH;
                        break;
                    } 
                    else if(isdigit(c)){
                        state = S_INT;
                        value[length] = c;
                        length++;
                        break;
                    } else if(c == '"') {
                        state = S_STRING_START;
                        break;
                    } 
                    else{
                        state = S_ERROR;
                        break;
                    }

                break; 

            case(S_MINUS):
                if(c == '>'){
                    token.type = TOKEN_FUNCTION_TYPE;
                    return token;
                } else {
                    return_back(c, file);  // Misto pouziti putc, fputc, unget tohle posune position pointer o znak zpet
                    token.type = TOKEN_MINUS;
                    return token;
                }
                break;

            case(S_QUESTION_MARK):
                if(c == '?'){
                    token.type = TOKEN_DOUBLE_QUESTION_MARK;
                    return token;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_QUESTION_MARK;
                    return token;
                }
                break; 
            
            case(S_UNDERSCORE):
                return_back(c, file);
                if(!isalnum(c) && c != '_'){        // Nemuze to byt ID, je to jen podtrzitko
                    token.type = TOKEN_UNDERSCORE;
                    return token;
                } else {                            // ID - 
                    state = S_ID;
                    value[length] = c;
                    length++;
                }
                break; 
            
            case(S_ID):
                if(c == '?'){   
                    value[length] = c;
                    length++;

                    int type = check_id_types(value);
                    switch(type){
                        case 0:
                            // Prislo ID s ?, jestli to neni jeden z typu, je to chyba
                            token.type = TOKEN_ERROR;
                            return token;
                            break;
                        case 1:
                            token.type = TOKEN_TYPE_FLOAT;
                            return token;
                            break;
                        case 2:
                            token.type = TOKEN_TYPE_INT;
                            return token;
                            break;
                        case 3:
                            token.type = TOKEN_TYPE_STRING;
                            return token;
                            break;
                    }


                    int type = check_id_types(value);
                    switch(type){
                        case 0:
                            // Prislo ID s ?, jestli to neni jeden z typu, je to chyba
                            token.type = TOKEN_ERROR;
                            return token;
                            break;
                        case 1:
                            token.type = TOKEN_TYPE_FLOAT;
                            return token;
                            break;
                        case 2:
                            token.type = TOKEN_TYPE_INT;
                            return token;
                            break;
                        case 3:
                            token.type = TOKEN_TYPE_STRING;
                            return token;
                            break;
                    }

                } else if(!isalnum(c) && c != '_'){     // znema log. operace (bylo || ale nefungovalo to :( )
                    return_back(c, file);

                    // Zkontroluj jestli to jsou keywords
                    int keyword = check_keywords(value);
                    switch(keyword){
                        case 0:
                            token.type        = TOKEN_ID;
                            token.value = malloc(length+1);
                            if(token.value == NULL)
                            {
                                fprintf(stderr, "MALLOC FAIL\n");
                                exit(INTER_ERROR);
                            }
                            strcpy(token.value, value);
                            token.valueLength = length; 
                            return token;
                            break;      // Zbytecny, ale whatever
                        case 1: 
                            token.type = TOKEN_KW_DOUBLE;
                            return token;
                            break;
                        case 2:
                            token.type = TOKEN_KW_ELSE;
                            return token;
                            break;
                        case 3:
                            token.type = TOKEN_KW_FUNC;
                            return token;
                            break;
                        case 4:
                            token.type = TOKEN_KW_IF;
                            return token;
                            break;
                        case 5:
                            token.type = TOKEN_KW_INT;
                            return token;
                            break;
                        case 6:
                            token.type = TOKEN_KW_LET;
                            return token;
                            break;
                        case 7:
                            token.type = TOKEN_KW_NIL;
                            return token;
                            break;
                        case 8:
                            token.type = TOKEN_KW_RETURN;
                            return token;
                            break;
                        case 9:
                            token.type = TOKEN_KW_STRING;
                            return token;
                            break;
                        case 10:
                            token.type = TOKEN_KW_VAR;
                            return token;
                            break;
                        case 11:
                            token.type = TOKEN_KW_WHILE;
                            return token;
                            break;
                    }
                } else {
                    value[length] = c;
                    length++;
                }
                break; 
            
            case(S_EXCLAMATION_MARK):
                if(c == '='){
                    token.type = TOKEN_NOT_EQUAL;
                    return token;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_EXCLAMATION_MARK;
                    return token;
                }
                break; 
            
            case(S_ASSIGN):
                if(c == '='){
                    token.type = TOKEN_EQUAL;
                    return token;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_ASSIGN;
                    return token;
                }
                break; 
                
            case(S_GT):
                if(c == '='){
                    token.type = TOKEN_GTE;
                    return token;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_GT;
                    return token;
                }
                break; 

            case(S_LT):
                if(c == '='){
                    token.type = TOKEN_LTE;
                    return token;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_LT;
                    return token;
                }
                break; 
// KOMENTARE
            case(S_SLASH):
                if(c == '/'){
                    state = S_LINE_COMMENT;
                    break;
                } else if(c == '*'){
                    state = S_BLOCK_COMMENT;
                    blockComms++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_SLASH;
                    return token;
                }
                break; 

            case(S_LINE_COMMENT):                   // Dojed na konec radku nebo souboru
                while(c != EOF && c != '\n'){
                    c = fgetc(file);
                }

                if(c == EOF){
                    token.type = TOKEN_EOF;
                    return token;
                } else {        // \n
                    state = S_START;
                    break;
                }
                break; 

            case(S_BLOCK_COMMENT):
                while(c != EOF && c != '*'){
                    if(c == '/'){               // Vnoreny komentar
                        c = fgetc(file);
                        if(c == '*'){
                            blockComms++;
                        }
                    }
                    c = fgetc(file);
                }

                if(c == EOF){
                    token.type = TOKEN_EOF;
                    return token;
                } else {    // *
                    state = S_BLOCK_MAYBE_END;
                    break;
                }
                break; 

            case(S_BLOCK_MAYBE_END):
                if(c == '/'){
                    blockComms--;
                    if(!blockComms){    // Pokud skoncily vnorene komentare/byl samotny
                        c = fgetc(file);
                        state = S_START;
                        break;
                    } else {
                        state = S_BLOCK_COMMENT;
                        break;
                    }
                }
                else 
                    state = S_BLOCK_COMMENT;
                break; 
// CISLA
            case(S_INT):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else if(c == '.'){
                    state = S_DOUBLE;
                    value[length] = c;
                    length++;
                    break;
                } else if(c == 'e' || c == 'E'){
                    state = S_INT_EXP;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_INT;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break;

            case(S_INT_EXP):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else if(c == '+' || c == '-'){
                    state = S_INT_EXP_PM;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_INT_EXP;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break; 
                
            case(S_INT_EXP_PM):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    state = S_INT_EXP_PM_OK;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break; 

            case(S_INT_EXP_PM_OK):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_INT_EXP_PM;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break; 

            case(S_DOUBLE):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else if(c == 'e' || c == 'E') {
                    state = S_DOUBLE_EXP;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_DOUBLE;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break; 

            case(S_DOUBLE_EXP):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else if(c == '+' || c == '-'){
                    state = S_DOUBLE_EXP_PM;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_DOUBLE_EXP;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break;
         
            case(S_DOUBLE_EXP_PM):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    state = S_DOUBLE_EXP_PM_OK;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break; 

            case(S_DOUBLE_EXP_PM_OK):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_DOUBLE_EXP_PM;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break; 

// STRING
            case(S_STRING_START):
                while(c != '"' && c != '\\' && c != EOF && c != '\n'){
                    value[length] = c;
                    length++;
                    c = fgetc(file);
                }

                if(c == '"'){
                    state = S_STRING;
                    break;
                } else if(c == '\\'){       // ESCAPE sekvence
                    state = S_BS;
                    value[length] = c;
                    length++;
                    break;
                } else if (c == EOF || c == '\n'){  
                    token.type = TOKEN_ERROR;
                    return token;
                } else {
                    state = S_STRING_FILL;
                    break;
                }
                break; 

            case(S_STRING_FILL):
                while(c != '"' && c != '\\' && c != EOF && c != '\n'){
                    value[length] = c;
                    length++;
                    c = fgetc(file);
                }

                if(c == '"'){
                    token.type = TOKEN_STRING;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                    {
                        fprintf(stderr, "MALLOC FAIL\n");
                        exit(INTER_ERROR);
                    }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                } else if(c == '\\'){       // ESCAPE sekvence
                    state = S_BS;
                    value[length] = c;
                    length++;
                    break;
                } else if (c == EOF || c == '\n'){  
                    token.type = TOKEN_ERROR;
                    return token;
                } 
                break;

            case(S_BS):
                if (c == 'u'){
                    state = S_BS_U;
                    break;
                }
                else if(c == 'n'){
                    state = S_STRING_START;
                    break;
                }
                else if(c == 'r'){
                    state = S_STRING_START;
                    break;
                }
                else if(c == 't'){
                    state = S_STRING_START;
                    break;
                }
                else if(c == '\\'){
                    state = S_STRING_START;
                    break;
                }
                else if(c == '"'){
                    state = S_STRING_START;
                    break;
                }
                else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break;

            case(S_BS_U):
                if(c == '{'){
                    state = S_BS_UC;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }

            case(S_BS_UC):
                if((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')){
                    value[length] = c;
                    length++;
                    hexLength++;
                    state = S_BS_UC_DD;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break;

            case(S_BS_UC_DD):
                while(c != '}'){   // 8???? <= ????
                    if((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')){
                        value[length] = c;
                        length++;
                        hexLength++;
                        c = fgetc(file);
                    } else {
                        token.type = TOKEN_ERROR;
                        return token;
                    }
                    if(hexLength > 8){
                        token.type = TOKEN_ERROR;
                        return token;
                    }
                }
                state = S_STRING_FILL; // Tady uz to nasbiralo co potrebuje, nebo hodilo chybu
                break;

            case(S_STRING):
                if(c == '"'){
                    state = S_ML_STRING_FILL;
                    break;
                } else {
                    return_back(c, file);
                    token.type = TOKEN_STRING;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                }
                break; 

// ML STRINGY
            case(S_ML_STRING_FILL):
                while(c != '"' && c != '\\' && c != EOF){
                    value[length] = c;
                    length++;
                    c = fgetc(file);
                }

                if(c == '"'){
                    state = S_ML_STRING_1;
                    break;
                } else if(c == '\\'){       // ESCAPE sekvence - tady muze byt \t, \n, \r normalne
                    state = S_ML_BS;
                    value[length] = c;
                    length++;
                    break;
                } else if (c == EOF){  
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break; 

            case(S_ML_BS):
                if(c == '\\'){
                    state = S_ML_STRING_FILL;
                    value[length] = c;
                    length++;
                    break;
                } else if(c == 'u'){
                    state = S_ML_BS_U;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break;

            case(S_ML_BS_U):
                if(c == '{'){
                    state = S_ML_BS_UC;
                    value[length] = c;
                    length++;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break;
//-----------
            case(S_ML_BS_UC):
                if((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')){
                    value[length] = c;
                    length++;
                    hexLength++;
                    state = S_ML_BS_UC_DD;
                    break;
                } else {
                    token.type = TOKEN_ERROR;
                    return token;
                }
                break;

            case(S_ML_BS_UC_DD):
                while(c != '}' || hexLength < 8){   // 8???? <= ????
                    if((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9')){
                        value[length] = c;
                        length++;
                        hexLength++;
                        c = fgetc(file);
                    } else {
                        token.type = TOKEN_ERROR;
                        return token;
                    }
                }
                state = S_ML_STRING_FILL; // Tady uz to nasbiralo co potrebuje, nebo hodilo chybu
                break;

            case(S_ML_STRING_1):
                if (c == '"'){
                    state = S_ML_STRING_2;
                    break;
                } else {
                    value[length] = '"';
                    value[length + 1] = c;
                    length += 2;
                    state = S_ML_STRING_FILL;
                    break;
                }
                break;  

            case(S_ML_STRING_2):
                if (c == '"'){
                    token.type = TOKEN_ML_STRING;
                    token.value = malloc(length+1);
                    if(token.value == NULL)
                        {
                            fprintf(stderr, "MALLOC FAIL\n");
                            exit(INTER_ERROR);
                        }
                    strcpy(token.value, value);
                    token.valueLength = length;
                    return token;
                } else {
                    value[length] = '"';
                    value[length + 1] = c;
                    length += 2;
                    state = S_ML_STRING_FILL;
                    break;
                }
                break; 

            default:  // Vsechno ostatni by mela byt chyba
                token.type = TOKEN_ERROR;
                //token.value = c;
                //token.valueLength = 1;
                return token;

        }
    }

    // EOF - Konec souboru
    token.type = TOKEN_EOF;
    return token;
}
