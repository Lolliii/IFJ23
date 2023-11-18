/*
IFJ Projekt 2023

Lineárně zřetězený seznam - zásobník (stack)

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
#include "expr_stack.h"

#include <stdio.h>
#include <string.h>

// Inicializace zásobníku = dynamické přiřazení paměti a nastavení vrcholu zásobníku na NULL
T_stack *stack_init(void)
{
    T_stack *stack = malloc(sizeof(T_stack));
    if(stack == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    stack->top = NULL;
    return stack;
}

// Přidání prvku na vrchol zásobníku (dynamická alokace paměti pro prvek)
// Data, která jsou uložena je token a index tokenu v precedenční tabulce (idx)
void stack_push(T_stack *stack, T_token token, char* value, prec_symb idx)
{
    T_elem *new_elem = (T_elem *) malloc(sizeof(T_elem));
    if(new_elem == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    new_elem->token = token;
    new_elem->value = malloc(token.valueLength+1);
    if(new_elem->value == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    strcpy(new_elem->value, value);
    new_elem->symb = idx;
    new_elem->next = stack->top;
    stack->top = new_elem;
}

// Zrušení prvku na vrcholu zásobníku a uklizení paměti
void stack_pop(T_stack *stack)
{
    T_elem *pop_elem = stack->top;
    stack->top = stack->top->next;
    free(pop_elem->value);
    free(pop_elem);
}

// Funkce vrací prvek na indexu v seznamu (tedy ne jenom vrchol zásobníku)
T_elem *stack_get_val(T_stack *stack, int index)
{
    T_elem *find_elem = stack->top;
    int cnt = 0;
    while(!(find_elem == NULL))
    {
        if(cnt == index)
            return find_elem;
        find_elem = find_elem->next;
        cnt++;
    }
    //Index přesáhl velikost zásobníku -> error
    fprintf(stderr, "ERROR: Stack error in expression\n");
    exit(EXPRESSION_ERROR);
}

// Vyprázdění zásobníku a dealokace paměti
void stack_empty(T_stack *stack)
{
    T_elem *pop_elem = stack->top;
    
    // Mazání prvků ze zásobníku
    while (pop_elem != NULL)
    {
        stack->top = pop_elem->next;
        free(pop_elem->value);
        free(pop_elem);
        pop_elem = stack->top;
    }
    // Smazání zásobníku
    free(stack);
}
