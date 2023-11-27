/*
IFJ Projekt 2023

Hlavičkový soubor pro lineární ADT zásobník

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include "scanner.h"
#include "parser_queue.h"

/* Indexace tokenů v precedenční tabulce (+ je na indexu 0, - na 1 atd...) */
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

    expressions,                    // Pro možnost přeskočení expressions
    e_id,                           // Po použití pravidla se na vrchol uloží některý typ z expressions
    e_num,
    e_dbl,
    e_str,
    e_bool,
    e_id_exc,
    e_end,
} prec_symb;

/* Struktura prvku na zásobníku */
typedef struct telem
{
    T_token token;              // Data prvku (token)
    char *value;                // Řetězec v token.value (jinak mi to nefungovalo)
    prec_symb symb;             // Index tokenu v precedenční tabulce
    struct telem *next;         // Ukazatel na další prvek
}T_elem;

/* Struktura samotného zásobníku s ukazatelem na vrchol */
typedef struct tstack
{
    T_elem *top;
}T_stack;

/* Funkce pro inicializaci zásobíku (alokoce paměti a nastavení ukazatele na NULL)
   Bez parametrů
   Vrací ukazatel na vytvořený zásobník*/
T_stack *stack_init(void);

/* Funkce pro vložení prvku na vrchol zásobníku
   Parametry jsou: ukazatel na zásobník, token a jeho hodnota, kterou chceme vložit a index tokenu v precedenční tabulce
   Bez návratové hodnoty*/
void stack_push(T_stack *stack, T_token token, char *value, prec_symb idx);

/* Funkce pro odstranění prvku z vrcholu zásobníku
   Parametrem je ukazatel na zásobník
   Bez návratové hodnoty*/
void stack_pop(T_stack *stack);

/* Funkce pro získání prvku z jakéhokoliv místa na zásobníku
   Parametry jsou: ukazatel na zásobník a index prvku, který chceme získat
   Vrací ukazatel na prvek zásobníku, když index přesáhne velikost zásobníku, funkce ukončí program*/
T_elem *stack_get_val(T_stack *stack, int index);

/* Funkce pro vyprázdění zásobníku a uvolnění paměti
   Parametrem je ukazatel na zásobník
   Bez návratové hodnoty*/
void stack_empty(T_stack *stack);

#endif
