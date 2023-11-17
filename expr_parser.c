/*
IFJ Projekt 2023

EXPRESSION PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "error.h"
#include "scanner.h"
#include "expr_stack.h"
#include "expr_parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Precedenční tabulka:
const char preced_tab [20][20] = {
/* +    -    *    /    !    (    )    <   <=    >   >=   ==   !=    id  int  dou  str  ??    $*/
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', '<', '>', '>'},
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},
{ '>', '>', '>', '>', 'x', '<', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'x'},
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '>', 'x', 'x', 'x', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', '<', '>'},
{ '<', '<', '<', '<', '<', '<', 'x', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e'}
};

//Fce pro daný token vrátí jeho idex v precedenční tabulce
prec_symb get_prec_value(T_token token)
{
    switch (token.type)
    {
    case TOKEN_PLUS:
        return prec_add;
    case TOKEN_MINUS:
        return prec_sub;
    case TOKEN_MUL:
        return prec_mul;
    case TOKEN_SLASH:
        return prec_divi;
    case TOKEN_EXCLAMATION_MARK:
        return prec_exc;
    case TOKEN_L_RND:
        return prec_l_brac;
    case TOKEN_R_RND:
        return prec_r_brac;
    case TOKEN_LT:
        return prec_lt;
    case TOKEN_LTE:
        return prec_lt_eq;
    case TOKEN_GT:
        return prec_gt;        
    case TOKEN_GTE:
        return prec_gt_eq;        
    case TOKEN_EQUAL:
        return prec_eq;        
    case TOKEN_NOT_EQUAL:
        return prec_n_eq;
    case TOKEN_ID:
        return prec_id;
    case TOKEN_TYPE_ID:
        return prec_id;
    case TOKEN_INT:
        return prec_num;
    case TOKEN_TYPE_FLOAT:
        return prec_dbl;
    case TOKEN_STRING:
        return prec_str;
    case TOKEN_DOUBLE_QUESTION_MARK:
        return prec_que;
    //TODO dodělat ukončovací symbol
    default:
        return prec_end;
    }
}

// Parsování výrazů TODO
void expr_parser(FILE* file)
{
    T_stack *stack = stack_init();
    T_token init_tok;

    // Na vrchol zásobníku vložím počáteční symbol $
    stack_push(stack, init_tok, prec_end);
    T_elem *stack_top = stack_find(stack, 0);
    
    // Získání tokenu z výrazu TODO, když jich parser pošle více, asi přes nějaký seznam
    T_token first_tok = getNextToken(file);

    while(1)
    {
        //printf("%i", get_prec_value(first_tok));
        //printf("%i", stack_top->symb);

        switch (preced_tab[get_prec_value(first_tok)][stack_top->symb])
        {

        case '<':
            printf("<\n");
            break;
        case '>':
            printf(">\n");
            break;
        case '=':
            printf("=\n");
            break;
        case 'e':
            printf("endd");
            break;
        }
        
        stack_push(stack, first_tok, get_prec_value(first_tok));
        first_tok = getNextToken(file);
        stack_top = stack_find(stack, 0);
        
        if(first_tok.type == TOKEN_EOF)
            break;
    }
    stack_empty(stack);
}
