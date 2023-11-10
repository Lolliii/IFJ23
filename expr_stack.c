#include <stdio.h>
#include "scanner.h"
#include "expr_stack.h"

T_stack *stack_init()
{
    T_stack *stack = malloc(sizeof(T_stack));
    if(stack == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(99);
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void stack_push(T_stack *stack, T_token token, prec_symb term)
{
    T_elem *new_elem = (T_elem *) malloc(sizeof(T_elem));
    if(new_elem == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(99);
    }
    new_elem->token = token;
    new_elem->symb = term;
    new_elem->next = stack->top;
    stack->top = new_elem;
    stack->size++;
}

void stack_pop(T_stack *stack)
{
    stack->size--;

    T_elem *pop_elem = stack->top;
    stack->top = stack->top->next;
    free(pop_elem);
}

T_elem *stack_find(T_stack *stack, int index)
{
    T_elem *find_elem = stack->top;
    int cnt = 0;
    do
    {
        if(cnt == index)
            break;
        find_elem = find_elem->next;
        cnt++;

    }
    while(!(find_elem->next == NULL));
    return find_elem;
}

void stack_empty(T_stack *stack)
{
    T_elem *elem = stack->top;
    
    while (elem != NULL)
    {
        stack->top = elem->next;
        free(elem);
        elem = stack->top;
    }
    free(stack);
}