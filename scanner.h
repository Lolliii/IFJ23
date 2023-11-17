/*
IFJ Project
@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef SCANNER_H
#define SCANNER_H

#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // kvuli uint_32

// Stavy -> Vsechny krome uplne konecnych stavu (pouze "pruchozi")
#define S_ERROR                 100
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
#define S_STRING                116
#define S_STRING_FILL           99
#define S_ML_BS                 117     // Multi-Line String
#define S_ML_BS_U               118     // \u
#define S_ML_BS_UC              119     // \u{ duplikace
#define S_ML_BS_UC_DD           120     // \u{dd
#define S_ML_STRING_FILL        123
#define S_ML_STRING_1           124
#define S_ML_STRING_2           125
#define S_ML_STRING             126
#define S_INT                   127
#define S_INT_EXP               128
#define S_INT_EXP_PM            129
#define S_INT_EXP_PM_OK         130
#define S_DOUBLE                131
#define S_DOUBLE_EXP            132
#define S_DOUBLE_EXP_PM         133
#define S_DOUBLE_EXP_PM_OK      134
#define S_SLASH                 135
#define S_BLOCK_COMMENT         136
#define S_LINE_COMMENT          137
#define S_BLOCK_MAYBE_END       138
//#define S_BLOCK_COMMENT_END     137

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

void printTokenName(T_token token);

int check_keywords(char *check);

void return_back(char c, FILE* file);

void error_caller(int error_code);

T_token getNextToken(FILE* file);

#endif
