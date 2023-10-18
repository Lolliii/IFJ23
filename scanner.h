/*
IFJ Project
@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h> // kvuli uint_32
#include "error.h"

// Stavy -> Vsechny krome uplne konecnych stavu (pouze "pruchozi")
// #define S_WHITESPACE            100
#define S_START                 101
#define S_MINUS                 102
#define S_QUESTION_MARK         103
#define S_UNDERSCORE            104
#define S_ID                    105
#define S_EXCLAMATION_MARK      106
#define S_ASSIGN                107
#define S_GT                    108
#define S_LT                    109
#define S_STRING_START          110    
#define S_BS                    111     // BS == BACKSLASH
#define S_BS_U                  112     // \u
#define S_BS_UC                 113     // \u{
#define S_BS_UC_DD              114     // \u{dd
//#define S_BS_UC                 115     // \u{ duplikace
#define S_STRING                116
#define S_ML_BS                 117     // Multi-Line String
#define S_ML_BS_U               118     // \u
//#define S_ML_BS_UC              119     // \u{ duplikace
#define S_ML_BS_UC_DD           120     // \u{dd
#define S_ML_BS_UC              123     // \u{
#define S_ML_STRING_1           124
#define S_ML_STRING_2           125
#define S_ML_STRING             126
#define S_INT                   127
#define S_INT_EXP               128
#define S_INT_EXP_PM            129
// ?? Pridat jeste jeden stav ?
#define S_DOUBLE                130
#define S_DOUBLE_EXP            131
#define S_DOUBLE_EXP_PM         132
// ?? Pridat jeste jeden stav ?
#define S_SLASH                 133
#define S_BLOCK_COMMENT         134
#define S_LINE_COMMENT          135
#define S_BLOCK_MAYBE_END       136
#define S_BLOCK_COMMENT_END     137

// Klicova slova
typedef enum {
    KEYWORD_DOUBLE,
    KEYWORD_ELSE,
    KEYWORD_FUNC,
    KEYWORD_IF,
    KEYWORD_INT,
    KEYWORD_LET,
    KEYWORD_NIL,
    KEYWORD_RETURN,
    KEYWORD_STRING,
    KEYWORD_VAR,
    KEYWORD_WHILE
} T_keyword;

// Tokeny (Konecne stavy)
// ?? Pridat EOL, WHITESPACE, KOMENT_END????
typedef enum {
    TOKEN_ERROR,
    TOKEN_EOF,                          // EOF
    TOKEN_COMMA,                        // , 
    TOKEN_COLON,                        // :
    TOKEN_PLUS,                         // +
    TOKEN_MINUS,                        // -
    TOKEN_FUNCTION_TYPE,                // ->
    TOKEN_MUL,                          // *
    TOKEN_QUESTION_MARK,                // ? 
    TOKEN_DOUBLE_QUESTION_MARK,         // ?? 
    TOKEN_UNDERSCORE,                   // _
    TOKEN_ID,                           // ID
    TOKEN_TYPE_ID,                      // ? - type id
    TOKEN_EXCLAMATION_MARK,             // !
    TOKEN_NOT_EQUAL,                    // != 
    TOKEN_ASSIGN,                       // =
    TOKEN_EQUAL,                        // == 
    TOKEN_GT,                           // >
    TOKEN_GTE,                          // >=
    TOKEN_LT,                           // < less than
    TOKEN_LTE,                          // <= less than or equal
    TOKEN_L_CURL,                       // {
    TOKEN_R_CURL,                       // }
    TOKEN_L_SQR,                        // [
    TOKEN_R_SQR,                        // ]
    TOKEN_R_RND,                        // )
    TOKEN_L_RND,                        // (
    TOKEN_SINGLE_QUOTE,                 // '
    TOKEN_SLASH,                        // /      
    // TOKEN_LINE_CMT,                     // //
    // TOKEN_BLOCK_CMT,                    // /* */
    TOKEN_BSLASH,                       // backslash 
    TOKEN_STRING,                       // ""
    TOKEN_ML_STRING,                    // """ """
    TOKEN_INT,
    TOKEN_INT_EXP,
    TOKEN_INT_EXP_PM,
    TOKEN_DOUBLE,
    TOKEN_DOUBLE_EXP,
    TOKEN_DOUBLE_EXP_PM,
    // Keywordy ??
    TOKEN_KEYWORD,                      // keyword
    TOKEN_TYPE_INT,                     // type int
    TOKEN_TYPE_FLOAT,                   // type float
    TOKEN_TYPE_STRING,                  // type string
} T_token_type;

typedef struct {
    T_token_type type;                  // typ tokenu
    char *value;
    uint32_t valueLength;               // Bude vyuzito pouze u stringu, var, cisel, ...
} T_token;

// TOHLE JESTE PREDELAM, nevim proc to je takhle bruh
/*
* Function compares given string with keywords
* Return values: 
* 0 -> strings don't match, 
* 1 -> strings match, 
* 2 -> type float, 
* 3 -> type int, 
* 4 -> type string 
*/
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
T_token get_token(FILE* file){

    T_token token;
    token.value = "\0";
    token.valueLength = 0;

    // Pomocne promenne pro naplnenni tokenu
    char value[1024] = "\0";                             // Tady to upravit na promennou delku nejak
    uint32_t length = 0;

    char c = getc(file);

    // Loop pro ziskani tokenu
    int state = S_START;
    while(c != EOF){
        if(state != S_START)
            c = getc(file);         // pokud znak nedostane po prvnim projiti finalni stav, je potreba nacist novy

        switch(state){
            case(S_START):      
            // Tady se to rozdeli podle toho jestli je to konecny stav nebo ne
            // Pokud z toho to stavu nelze uz nikam prejit, naplni a returnne se token

                    if(isspace(c)){         // bere i EOL
                        c = getc(file);     // při whitespace se to seklo v endless loopu
                        break;
                    }                // whitespaces
                    
                    if(c == ','){
                        token.type = TOKEN_COMMA;
                        return token;
                    }

                    if(c == ':'){
                        token.type = TOKEN_COLON;
                        return token;
                    }

                    if(c == '+'){
                        token.type = TOKEN_PLUS;
                        return token;
                    }

                    if(c == '-'){
                        state = S_MINUS;
                        break;
                    }

                    if(c == '*'){
                        token.type = TOKEN_MUL;
                        return token;
                    }

                    if(c == ':'){
                        token.type = TOKEN_COLON;
                        return token;
                    }

                    if(c == '?'){
                        state = S_QUESTION_MARK;
                        break;
                    }

                    if(c == '_'){
                        state = S_UNDERSCORE;
                        break;
                    }

                    if(isalpha(c)){
                        state = S_ID;
                        value[length] = c;
                        length++;
                        break;
                    }

                    if(c == '!'){
                        state = S_EXCLAMATION_MARK;
                        break;
                    }

                    if(c == '='){
                        state = S_ASSIGN;
                        break;
                    }

                    if(c == '>'){
                        state = S_GT;
                        break;
                    }

                    if(c == '<'){
                        state = S_LT;
                        break;
                    }

                    if(c == '}'){
                        token.type = TOKEN_R_CURL;
                        return token;
                    }

                    if(c == '{'){
                        token.type = TOKEN_L_CURL;
                        return token;
                    }

                    if(c == ']'){
                        token.type = TOKEN_R_SQR;
                        return token;
                    }

                    if(c == '['){
                        token.type = TOKEN_L_SQR;
                        return token;
                    }

                    if(c == ')'){
                        token.type = TOKEN_R_RND;
                        return token;
                    }

                    if(c == '('){
                        token.type = TOKEN_L_RND;
                        return token;
                    }

                    if(c == 39){        // Uvozovka ASCII
                        token.type = TOKEN_SINGLE_QUOTE;
                        return token;
                    }

                    if(c == '/'){
                        state = S_SLASH;
                        break;
                    }

                    if(isdigit(c)){
                        state = S_INT;
                        value[length] = c;
                        length++;
                        break;
                    }

                break; 

            case(S_MINUS):
                if(c == '>'){
                    token.type = TOKEN_FUNCTION_TYPE;
                    return token;
                } else {
                    putc(c, file);
                    token.type = TOKEN_MINUS;
                    return token;
                }
                break;

            case(S_QUESTION_MARK):
                if(c == '?'){
                    token.type = TOKEN_DOUBLE_QUESTION_MARK;
                    return token;
                } else {
                    putc(c, file);
                    token.type = TOKEN_QUESTION_MARK;
                    return token;
                }
                break; 
            
            case(S_UNDERSCORE):
                putc(c, file);
                if(!isalnum(c) || c != '_'){        // Nemuze to byt ID, je to jen podtrzitko
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
                    token.type        = TOKEN_TYPE_ID;
                    token.value       = value;
                    token.valueLength = length; 
                    return token;
                } else if(!isalnum(c) && c != '_'){     // znema log. operace (bylo || ale nefungovalo to :( )
                    putc(c, file);
                    token.type        = TOKEN_ID;
                    token.value       = value;
                    token.valueLength = length; 
                    return token;
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
                    putc(c, file);
                    token.type = TOKEN_EXCLAMATION_MARK;
                    return token;
                }
                break; 
            
            case(S_ASSIGN):
                if(c == '='){
                    token.type = TOKEN_EQUAL;
                    return token;
                } else {
                    putc(c, file);
                    token.type = TOKEN_ASSIGN;
                    return token;
                }
                break; 
                
            case(S_GT):
                if(c == '='){
                    token.type = TOKEN_GTE;
                    return token;
                } else {
                    putc(c, file);
                    token.type = TOKEN_GT;
                    return token;
                }
                break; 

            case(S_LT):
                if(c == '='){
                    token.type = TOKEN_LTE;
                    return token;
                } else {
                    putc(c, file);
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
                    break;
                } else {
                    putc(c, file);
                    token.type = TOKEN_SLASH;
                    return token;
                }
                break; 

            case(S_LINE_COMMENT):                   // Dojed na konec radku nebo souboru
                while(c != EOF || c != '\n'){
                    c = getc(file);
                }

                if(c == EOF){
                    token.type = TOKEN_EOF;
                    return token;
                }
                break; 

            case(S_BLOCK_COMMENT):
                while(c != EOF || c != '*'){
                    c = getc(file);
                }

                if(c == EOF){
                    token.type = TOKEN_ERROR;
                    return token;
                } else {    // *
                    state = S_BLOCK_MAYBE_END;
                    break;
                }
                break; 

            case(S_BLOCK_MAYBE_END):
                if(c == '/') 
                    state = S_START;
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
                    putc(c, file);
                    token.type = S_INT;
                    token.value = value;
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
                    putc(c, file);
                    token.type = S_INT_EXP;
                    token.value = value;
                    token.valueLength = length;
                    return token;
                }
                break; 
                
    // !!!!!!!!! tady mozna upravit jeste aby na konci nemohlo by jen +/- 
            case(S_INT_EXP_PM):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else {
                    putc(c, file);
                    token.type = S_INT_EXP_PM;
                    token.value = value;
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
                    putc(c, file);
                    token.type = S_DOUBLE;
                    token.value = value;
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
                    putc(c, file);
                    token.type = S_DOUBLE_EXP;
                    token.value = value;
                    token.valueLength = length;
                    return token;
                }
                break;

    // !!!!!!!!! tady mozna upravit jeste aby na konci nemohlo by jen +/-             
            case(S_DOUBLE_EXP_PM):
                if(isdigit(c)){
                    value[length] = c;
                    length++;
                    break;
                } else {
                    putc(c, file);
                    token.type = S_DOUBLE_EXP_PM;
                    token.value = value;
                    token.valueLength = length;
                    return token;
                }
                break; 

// STRING
            /*
            case():
                break; 
            case():
                break; 
            */






            default:  // Vsechno ostatni by mela byt chyba
                token.type = TOKEN_ERROR;
                //token.value = c;
                token.valueLength = 1;
                return token;
        }
    }

    // EOF - Konec souboru
    token.type = TOKEN_EOF;
    return token;
}