/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include "scanner.h"

// indexace tokenů v precedenční tabulce (+ je na indexu 0 atd...)
typedef enum {
    prec_add,
    prec_sub,
    prec_mul,
    prec_divi,
    prec_exc,
    prec_l_brac,
    prec_r_brac,
    prec_lt,
    prec_lt_eq,
    prec_gt,
    prec_gt_eq,
    prec_eq,
    prec_n_eq,
    prec_id,
    prec_num,
    prec_dbl,
    prec_str,
    prec_que,
    prec_end,

    expressions,
    e_id,
    e_num,
    e_dbl,
    e_str,
    e_end,
} prec_symb;

typedef struct telem
{
    T_token token;
    prec_symb symb;
    struct telem *next;
}T_elem;

typedef struct tstack
{
    T_elem *top;
}T_stack;

T_stack *stack_init();

void stack_push(T_stack *stack, T_token token, prec_symb idx);

void stack_pop(T_stack *stack);

T_elem *stack_find(T_stack *stack, int index);

void stack_empty(T_stack *stack);

#endif