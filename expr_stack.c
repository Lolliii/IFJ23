/*
IFJ Projekt 2023

Implementace lineárního ADT zásobník

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
#include "expr_stack.h"

#include <stdio.h>
#include <string.h>

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


void stack_push(T_stack *stack, T_token token, char* value, prec_symb idx)
{
    T_elem *new_elem = (T_elem *) malloc(sizeof(T_elem));
    if(new_elem == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    new_elem->token = token;
    // Alokace paměti pro dynamický řetězec z tokenu -> token.value
    new_elem->value = malloc(token.valueLength + 1);
    if(new_elem->value == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    strcpy(new_elem->value, value);
    new_elem->symb = idx;
    // Nový prvek je vrcholem zásobníku a zřetězení s předchozím vrcholem
    new_elem->next = stack->top;
    stack->top = new_elem;
}

void stack_pop(T_stack *stack)
{
    T_elem *pop_elem = stack->top;
    stack->top = stack->top->next;
    // Uvolnění místa pro dynamický řetězec a samotný prvek
    free(pop_elem->value);
    free(pop_elem);
}

T_elem *stack_get_val(T_stack *stack, int index)
{
    T_elem *find_elem = stack->top;
    int cnt = 0;
    // Procházení zásobníkem dokuď nenajdeme daný prvek, nebo index nepřesáhne velikost zásobníku
    while(!(find_elem == NULL))
    {
        if(cnt == index)
            return find_elem;
        find_elem = find_elem->next;
        cnt++;
    }
    // Index přesáhl velikost zásobníku -> error
    // Nejčastěji nastavne při špatné práci se závorkami, např.: ((id))
    fprintf(stderr, "ERROR: Stack error in expression\n");
    exit(SYN_ERROR);
}

// Vyprázdění zásobníku a dealokace paměti
void stack_empty(T_stack *stack)
{
    T_elem *pop_elem = stack->top;
    
    // Postupné mazání prvků ze zásobníku
    while (pop_elem != NULL)
    {
        // Posunutí vrcholu zásobníku na následující prvek
        stack->top = pop_elem->next;
        free(pop_elem->value);
        free(pop_elem);
        pop_elem = stack->top;
    }
    // Uvolnění místa od zásobníku
    free(stack);
}
