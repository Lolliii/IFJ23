/*
IFJ Projekt 2023

Hlavičkový soubor pro parser na vyhodnocování výrazů

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "error.h"
#include "scanner.h"
#include "expr_stack.h"
#include "expr_parser.h"
#include "symtable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Funkce vybere typu tokenu (token.type) odpovídající index v precedenční tabulce
   Parametry jsou: token, ukazatel na příznak end_expr, ukazatel na frontu tokenů a ukazatel na soubor
   Funkce vrací číslo indexu v precedenční tabulce*/
prec_symb get_prec_value(T_token token, int *end_expr, T_queue *queue, FILE* file);

/* Funkce pro vybrání pravidla pro prvek z vrcholu zásobníku
   Parametry jsou: ukazatele na zásobník a na prvek z vrcholu zásobníku
   Bez návratové hodnoty*/
void reduce_rule(T_stack *stack, T_elem *stack_top, Tlist *sym_list);

/* Funkce, která zkontroluje, zdali je levý a pravý operand literál nebo ID
   Parametry jsou levý a pravý operand operátoru, který je nejvrchnějším terminálem na zásobníku 
   Bez návratové hodnoty*/
void check_two_operands(T_elem l_op, T_elem r_op);    

int check_e_id(T_elem *element, Tlist *sym_list);    //hledani typu id
void check_id_exc(T_elem *element, Tlist *sym_list);  //funkci nepouzivame

//kontroly komatibility
void id_rule_plus(T_elem *l_op, T_elem *r_op);        
void id_rule_min_mul(T_elem *l_op, T_elem *r_op);     
void id_rule_div(T_elem *l_op, T_elem *r_op);
void id_rule_rela(T_elem *l_op, T_elem *r_op);
void id_rule_rela_equal(T_elem *l_op, T_elem *r_op);
void id_rule_nil_coal(T_elem *l_op, T_elem *r_op);


/* Funkce pro aplikaci odpovídajícího pravidla
   Parametrem je ukazatel na zásobník
   Bez návratové hodnoty, v případě Syntaktické/Sématické chyby funkce ukončí program*/
void rule_plus(T_stack *stack, Tlist *sym_list);
void rule_min_mul(T_stack *stack, Tlist *sym_list);
void rule_div(T_stack *stack, Tlist *sym_list);
void rule_rela(T_stack *stack, Tlist *sym_list);
void rule_rela_equal(T_stack *stack, Tlist *sym_list);
void rule_nil_coal(T_stack *stack, Tlist *sym_list);

/* Funkce podle indexu prvku z vrcholu zásobníku a aktualního tokenu vybere operaci (<, >, =, x)
   Parametry jsou ukazatel na soubor a na frontu
   Bez návratové hodnoty, v případě operace 'x' funkce ukončí program*/
T_token_type expr_parser(FILE* file, T_queue *queue, Tlist *sym_list);

#endif
