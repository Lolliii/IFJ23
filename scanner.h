/*
IFJ Project
@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "error.h"

// Stavy -> Vsechny krome uplne konecnych stavu (pouze "pruchozi")
#define S_WHITESPACE            100
#define S_START                 101
#define S_MINUS                 102
#define S_QUESTION_MARK         103
#define S_UNDERSCORE            104
#define S_ID                    105
#define S_EXCLAM_MARK           106
#define S_ASSIGN                107
#define S_GT                    108
#define S_LT                    109
#define S_STRING_START          110    
#define S_BS                    111     // BS == BACKSLASH
#define S_BS_U                  112     // \u
#define S_BS_UC                 113     // \u{
#define S_BS_UC_DD              114     // \u{dd
#define S_BS_UC                 115     // \u{
#define S_STRING                116
#define S_ML_BS                 117     // Multi-Line String
#define S_ML_BS_U               118     // \u
#define S_ML_BS_UC              119     // \u{
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
// ?? Pridat EOF, EOL, WHITESPACE, KOMENT_END????
typedef enum {
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
    TOKEN_EXCLAM_MARK,                  // !
    TOKEN_NOT_EQ,                       // != 
    TOKEN_ASSIGN,                       // =
    TOKEN_EQUAL,                        // == 
    TOKEN_GT,                           // >
    TOKEN_GE,                           // >=
    TOKEN_LT,                           // < less than
    TOKEN_LE,                           // <= less than or equal
    TOKEN_L_CURL,                       // {
    TOKEN_R_CURL,                       // }
    TOKEN_L_SQR,                        // [
    TOKEN_R_SQR,                        // ]
    TOKEN_R_RND,                        // )
    TOKEN_L_RND,                        // (
    TOKEN_SINGLE_QUOTE,                 // '
    TOKEN_SLASH,                        // /      
    TOKEN_LINE_CMT,                     // //
    TOKEN_BLOCK_CMT,                    // /* */
    TOKEN_BSLASH,                       // \ 
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
    T_token_type type;                // typ tokenu
    //string attribute;               // hodnota tokenu
    char *attribute;
} T_token;

/*
 * Fills token given in an argument
 *
 * Returns:
 * 0 -> no error
 * 1 -> lexical error (error_caller prints error message)
*/
int get_tokens(T_token *token, FILE *file);


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
