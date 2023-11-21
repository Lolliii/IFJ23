/*
IFJ Projekt 2023

EXPRESSION PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

// #include "error.h"
// #include "expr_stack.h"
#include "expr_parser.h"

// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>


//Fce pro daný token vrátí jeho idex v precedenční tabulce
prec_symb get_prec_value(T_token token, int *zavorka)
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
        if(*zavorka == 0)
        {
            
            return prec_end;            
        }
        else
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
    
    case TOKEN_INT:
    case TOKEN_INT_EXP:
    case TOKEN_INT_EXP_PM:
        return prec_num;
    case TOKEN_TYPE_FLOAT:
    case TOKEN_DOUBLE:
    case TOKEN_DOUBLE_EXP:
    case TOKEN_DOUBLE_EXP_PM:
        return prec_dbl;

    case TOKEN_STRING:
    case TOKEN_ML_STRING:
        return prec_str;
    case TOKEN_DOUBLE_QUESTION_MARK:
        return prec_que;
    
    // TODO dodělat ukončovací symbol
    // asi jediný problém by nastal, kdyby přišlo ID, které již nepatří do výrazu
    // pak by stačilo se podívat na další token což by mělo být '=' pro přiřazení, nebo '(' pro funkci
    // a pak ty dva tokeny vrátit parseru
    // ale nejsem si tím úplně jistý...
    default:
        
        return prec_end;
    }
}

void rule_plus(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str || l_op->symb == e_id) && 
    (r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str || l_op->symb == e_id)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if(l_op->symb == e_str && r_op->symb == e_str)
    {
        printf("conca ");
        l_op->symb = e_str;
    }
    else if((l_op->symb == e_num && r_op->symb == e_num) || (l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        printf("+ ");
        l_op->symb = r_op->symb;    //zbytečné, ale pro naznačení
    }
    else if((l_op->symb == e_num || l_op->symb == e_dbl) && (r_op->symb == e_num || r_op->symb == e_dbl))
    {
        // Provedení konverze jednoho z operandu na DOUBLE
        printf("+ ");
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
        printf("+ ");
        l_op->symb = e_dbl;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
    
    stack_pop(stack);
    stack_pop(stack);
}

void rule_min_mul(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str || l_op->symb == e_id) && 
    (r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str || r_op->symb == e_id)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if((l_op->symb == e_num && r_op->symb == e_num) || (l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        l_op->symb = r_op->symb;    //zbytečné, ale pro naznačení
    }
    else if((l_op->symb == e_num || l_op->symb == e_dbl) && (r_op->symb == e_num || r_op->symb == e_dbl))
    {
        // Provedení konverze jednoho z operandu na DOUBLE
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
        l_op->symb = e_dbl;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
    
    stack_pop(stack);
    stack_pop(stack);
}

void rule_div(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str || l_op->symb == e_id) && 
    (r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str || r_op->symb == e_id)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if((l_op->symb == e_num && r_op->symb == e_num))
    {
        // Celočíselné dělení
        l_op->symb = e_num;
    }
    else if((l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        // Desetinné dělení
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
        l_op->symb = e_dbl;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
    
    if(strcmp(r_op->value, "0") == 0)
    {
        fprintf(stderr, "ERROR: Cannot divide by 0 \n");
        exit(OTHER_ERROR);
    }

    stack_pop(stack);
    stack_pop(stack);
}

void rule_rela(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_id || l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str) && 
    (r_op->symb == e_id || r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if((l_op->symb == e_num && r_op->symb == e_num) || 
            (l_op->symb == e_dbl && r_op->symb == e_dbl) || 
            (l_op->symb == e_str && r_op->symb == e_str))
    {
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO: potřeba Tabulky symbolů
        // můsí být stejné typy a bez nil (takže pro String? musí být před tím !)
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
}

void rule_rela_equal(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_id || l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str) && 
    (r_op->symb == e_id || r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if(((l_op->symb == e_num || l_op->symb == e_dbl) && (r_op->symb == e_num || r_op->symb == e_dbl)) ||
            (l_op->symb == e_str && r_op->symb == e_str))
    {
        // Pro Int,Dbl neob Dbl,Int provést implicitní konverzi
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO: potřeba Tabulky symbolů
        // můsí být také stejné typy (bez implicitní konverze) může být NIL type
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
}

// Nil coalescing operator
void rule_nil_coal(T_stack *stack)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    if(!((l_op->symb == e_id || l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str) && 
    (r_op->symb == e_id || r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str)))
    {
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
    else if((l_op->symb == e_num && r_op->symb == e_num) || (l_op->symb == e_dbl && r_op->symb == e_dbl) ||
            (l_op->symb == e_str && r_op->symb == e_str))
    {
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = r_op->symb;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id)
    {
        // TODO: potřeba Tabulky symbolů
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_id;
    }
    else
    {
        fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        exit(TYPE_COMP_ERROR);
    }
}

void reduce_rule(T_stack *stack, T_elem *stack_top)
{
    switch (stack_top->symb)
    {
    case prec_add:
        rule_plus(stack);
        break;
    case prec_sub:
        rule_min_mul(stack);
        printf("- ");
        break;
    case prec_mul:
        rule_min_mul(stack);
        printf("* ");
        break;
    case prec_divi:
        rule_div(stack);
        break;
        
    case prec_lt:
        printf("< ");
        rule_rela(stack);
        break;
    case prec_lt_eq:
        printf("<= ");
        rule_rela(stack);
        break;
    case prec_gt:
        printf("> ");
        rule_rela(stack);
        break;
    case prec_gt_eq:
        printf(">= ");
        rule_rela(stack);
        break;
    case prec_eq:
        printf("== ");
        rule_rela_equal(stack);
        break;
    case prec_n_eq:
        printf("!= ");
        rule_rela_equal(stack);
        break;
        
    case prec_exc:
        /* code */
        break;

    // Závorka
    case prec_r_brac:;
        T_elem *op = stack_get_val(stack, 1);
        stack_pop(stack);
        stack_pop(stack);
        stack_pop(stack);
        stack_push(stack, op->token, op->value, op->symb);
        break;
    
    case prec_que:
        printf("??");
        rule_nil_coal(stack);
        break;
    
    case prec_id:
        stack_top->symb = e_id;
        printf("%s, ", stack_top->value);
        break;
    case prec_num:
        stack_top->symb = e_num;
        printf("%s, ", stack_top->value);
        break;
    case prec_dbl:
        stack_top->symb = e_dbl;
        printf("%s, ", stack_top->value);
        break;
    case prec_str:
        stack_top->symb = e_str;
        printf("%s, ", stack_top->value);
        break;
    case prec_end:
        stack_top->symb = e_end;
        break;
    
    default:
        fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as an operands\n");
        exit(SYN_ERROR);
    }
}


// Parsování výrazů TODO
void expr_parser(FILE* file, T_queue *queue)
{
// Precedenční tabulka:
const char preced_tab [20][20] = {
/* +    -    *    /    !    (    )    <   <=    >   >=   ==   !=    id  int  dbl  str  ??    $*/
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', '<', '>', '>'},// +
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},// -
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},// *
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', '>', '>'},// /
{ '>', '>', '>', '>', 'x', '<', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// !
{ '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'x'},// (
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// )
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// <
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// <=
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// >
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// >=
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// ==
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', '>'},// !=
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// id
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// int
{ '>', '>', '>', '>', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// dbl
{ '>', 'x', 'x', 'x', '>', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', '>', '>'},// str
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', '<', '>'},// ??
{ '<', '<', '<', '<', '<', '<', 'x', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e'} // $
};
    int end = 1;
    int zavorka = 0;
    T_stack *stack = stack_init();
    T_token init_tok;
    init_tok.type = TOKEN_ASSIGN;      // MILAN - zatim takhle, compiler hazel error
    init_tok.valueLength = 1;

    // Na vrchol zásobníku vložím počáteční symbol $
    stack_push(stack, init_tok, "x", prec_end);
    T_elem *stack_top = stack_get_val(stack, 0);
    
    T_token token = getToken(queue, file);
    while(end)
    {
        if(token.type == TOKEN_L_RND)
            zavorka++;
        // Získání prvního terminálu ze zásobníku (přeskočení expressions)
        int idx = 0;
        stack_top = stack_get_val(stack, idx);
            while(stack_top->symb > expressions)
                stack_top = stack_get_val(stack, ++idx);

        // Získání indexů pro tabulku
        int idx_col = get_prec_value(token, &zavorka);
        int idx_row = stack_top->symb;

        switch (preced_tab[idx_row][idx_col])
        {
        // Operace shift -> přidej token na vrchol zásobníku
        case '<':
            stack_push(stack, token, token.value, idx_col);
            if(token.valueLength)
                free(token.value);
            token = getToken(queue, file);
            break;

        // Proveď redukci a aplikuj pravidlo
        case '>':
            reduce_rule(stack, stack_top);
            break;

        // Pro vyhodnocení závorek -> je to vlastně operace shift a zároveň redukce
        case '=':
            zavorka--;
            stack_push(stack, token, token.value, idx_col);
            stack_top = stack_get_val(stack, 0);
            reduce_rule(stack, stack_top);
            if(token.valueLength)
                free(token.value);
            token = getToken(queue, file);
            break;

        // Chyba podle precedenční tabulky
        case 'x':
            if((idx_col == prec_str && (idx_row == prec_sub || idx_row == prec_mul || idx_row == prec_divi)) ||
               (idx_row == prec_str && (idx_col == prec_sub || idx_col == prec_mul || idx_col == prec_divi)))
            {
                fprintf(stderr, "ERROR: Expression error, invalid data types\n");
                exit(TYPE_COMP_ERROR);
            }
            else
            {
                fprintf(stderr, "ERROR: Syntax error, prec table fail\n");
                exit(SYN_ERROR);
            }
        
        // Konec precedeneční analýzy
        case 'e':
            end = 0;
            queue_add(queue, token);
            break;
        }
    }
    stack_empty(stack);
}
