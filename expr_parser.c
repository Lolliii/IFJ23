/*
IFJ Projekt 2023

Implementace parseru pro vyhodnocování výrazů

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "expr_parser.h"

int check_e_id(T_elem *element, Tlist *list){
    // hledani v listu
    ListElement *frame = bSearch_all(list, element->value);
    if(frame != NULL){
        // hledani v strome
        bStrom *found = bsearch_one(frame->data, element->value);
        T_id *id_data = (T_id *)(found->data);
        // je inicializovana
        if(id_data->initialized){
            switch(id_data->type){
                case TOKEN_KW_INT:
                    element->symb = e_num;
                    break;
                case TOKEN_KW_STRING:
                    element->symb = e_str;
                    break;
                case TOKEN_KW_DOUBLE:
                    element->symb = e_dbl;
                    break;
                case TOKEN_TYPE_INT:
                    element->symb = e_num;
                    break;
                case TOKEN_TYPE_FLOAT:
                    element->symb = e_dbl;
                    break;
                case TOKEN_TYPE_STRING:
                    element->symb = e_str;
                    break;
                default:
                    error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
                    exit(UNDEF_UNINIT_VARIABLE_ERROR);
                    break;
            }
        // neni inicializovana
        } else {
            // id je s ?
            if (id_data->type == TOKEN_TYPE_INT || id_data->type == TOKEN_TYPE_FLOAT || id_data->type == TOKEN_TYPE_STRING)
            {
                element->symb = e_nil;
            } else {
                error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
                exit(UNDEF_UNINIT_VARIABLE_ERROR);
            }
        }
        return id_data->type;
    } else {
        // nenaslo se id
        error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
        exit(UNDEF_UNINIT_VARIABLE_ERROR);
    }
}

// zatim se to nepouziva
void check_id_exc(T_elem *l_op, Tlist *sym_list){
    // hledani v listu
    ListElement *search = bSearch_all(sym_list, l_op->value);
    if(search != NULL) {
        // hledani v strome
        bStrom *item = bsearch_one(search->data, l_op->value);
        T_id *id_item = (T_id *)item->data;
        // neni inicializovana
        if (!id_item->initialized) {
            error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
            exit(UNDEF_UNINIT_VARIABLE_ERROR);
        }

        switch (id_item->type)
        {
            case TOKEN_TYPE_INT:
                l_op->symb = e_num;
                break;
            case TOKEN_TYPE_FLOAT:
                l_op->symb = e_dbl;
                break;
            case TOKEN_TYPE_STRING:
                l_op->symb = e_str;
                break;
            default:
                false;
                break;
        }
    } else {
        error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
        exit(UNDEF_UNINIT_VARIABLE_ERROR);
    }
}

void id_rule_plus(T_elem *l_op, T_elem *r_op) {
    if ((l_op->symb == e_num && r_op->symb == e_num ) ||
        (l_op->symb == e_dbl && r_op->symb == e_dbl ) ||
        (l_op->symb == e_str && r_op->symb == e_str ))
    {
        l_op->symb = r_op->symb;
    } else if ((l_op->symb == e_num || l_op->symb == e_dbl) &&
        (r_op->symb == e_num || r_op->symb == e_dbl)){
        l_op->symb = e_dbl;
    } else {
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void id_rule_min_mul(T_elem *l_op, T_elem *r_op){
    if ((l_op->symb == e_num && r_op->symb == e_num ) ||
        (l_op->symb == e_dbl && r_op->symb == e_dbl ))
    {
        l_op->symb = r_op->symb;
    } else if ((l_op->symb == e_num || l_op->symb == e_dbl) &&
        (r_op->symb == e_num || r_op->symb == e_dbl)){
        l_op->symb = e_dbl;
    } else {
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void id_rule_div(T_elem *l_op, T_elem *r_op){
    if ((l_op->symb == e_num && r_op->symb == e_num ) ||
        (l_op->symb == e_dbl && r_op->symb == e_dbl ))
    {
        l_op->symb = r_op->symb;
    } else {
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void id_rule_rela(T_elem *l_op, T_elem *r_op){
    if ((l_op->symb == e_num && r_op->symb == e_num ) ||
        (l_op->symb == e_dbl && r_op->symb == e_dbl ) ||
        (l_op->symb == e_str && r_op->symb == e_str ))
    {
        l_op->symb = e_bool;
    } else {
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void id_rule_rela_equal(T_elem *l_op, T_elem *r_op){
    if (l_op->symb != e_bool){
        if ((l_op->symb == e_num && r_op->symb == e_num ) ||
            (l_op->symb == e_dbl && r_op->symb == e_dbl ) ||
            (l_op->symb == e_str && r_op->symb == e_str ) ||
            (l_op->symb == e_nil && r_op->symb == e_nil ) ||
            ((l_op->symb == e_num || l_op->symb == e_dbl || l_op->symb == e_str) && r_op->symb == e_nil) ||
            (l_op->symb == e_nil && (r_op->symb == e_num || r_op->symb == e_dbl || r_op->symb == e_str)))
        {
            l_op->symb = e_bool;
        } else {
            error_caller(TYPE_COMP_ERROR);
            exit(TYPE_COMP_ERROR);
        }
    }
}

void id_rule_nil_coal(T_elem *l_op, T_elem *r_op){
    if ((l_op->symb == e_num && r_op->symb == e_num ) ||
        (l_op->symb == e_dbl && r_op->symb == e_dbl ) ||
        (l_op->symb == e_str && r_op->symb == e_str ))
    {
        l_op->symb = l_op->symb;
    } else {
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

prec_symb get_prec_value(T_token token, int *end_expr, T_queue *queue, FILE* file)
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
    case TOKEN_ID_EM:
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
    case TOKEN_ID:;
        /* Pro token ID, je zapotřebí se podívat na další token pro případ,
           že ID bude součástí volání funkce: id(), nebo přiřazení: id=,
           v těchto dvou případech je ID bráno za ukončovací výraz a tokeny jsou vloženy do fronty*/
        T_token tmp = getToken(queue, file);
        if(tmp.type == TOKEN_ASSIGN || tmp.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
            queue_add(queue, tmp);
            *end_expr = 1;              // Příznak, že bylo nalezeno nestandardní ukončení výrazu
            return prec_que;
        }
        // ID je součástí výrazu, token navíc vlož do fronty
        queue_add(queue, tmp);
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
    case TOKEN_KW_NIL:
        return prec_nil;
    // Token, který není v precedenční tabulce, je brán za standardní ukončení výrazu
    default:    
        return prec_end;
    }
}

void check_two_operands(T_elem l_op, T_elem r_op)
{
    if(!((l_op.symb == e_num || l_op.symb == e_dbl || l_op.symb == e_str || l_op.symb == e_id || l_op.symb == e_id_exc || l_op.symb == e_nil) && 
    (r_op.symb == e_num || r_op.symb == e_dbl || r_op.symb == e_str || r_op.symb == e_id || r_op.symb == e_id_exc || r_op.symb == e_nil)))
    {
        // fprintf(stderr, "ERROR: Syntax error, need 2 literals or ID as operands\n");
        error_caller(SYN_ERROR);
        exit(SYN_ERROR);
    }
}

void rule_plus(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);

    if(l_op->symb == e_str && r_op->symb == e_str)
    {
        //printf("conca ");
        l_op->symb = e_str;
    }
    else if((l_op->symb == e_num && r_op->symb == e_num) ||
            (l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        //printf("+ ");
        l_op->symb = r_op->symb;    //zbytečné, ale pro naznačení
    }
    else if((l_op->symb == e_num || l_op->symb == e_dbl) &&
            (r_op->symb == e_num || r_op->symb == e_dbl))
    {
        // Provedení konverze jednoho z operandu na DOUBLE
        //printf("+ ");
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            check_e_id(l_op, sym_list);

            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                id_rule_plus(l_op, r_op);
                
            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_plus(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);
                id_rule_plus(l_op, r_op);
            }
        }
        

        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
        //printf("+ ");
        // l_op->symb = e_id;
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
    
    stack_pop(stack);
    stack_pop(stack);
}

void rule_min_mul(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);

    if((l_op->symb == e_num && r_op->symb == e_num) ||
       (l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        l_op->symb = r_op->symb;    //zbytečné, ale pro naznačení
    }
    else if((l_op->symb == e_num || l_op->symb == e_dbl) && 
            (r_op->symb == e_num || r_op->symb == e_dbl))
    {
        // Provedení konverze jednoho z operandu na DOUBLE
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            check_e_id(l_op, sym_list);
            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                id_rule_min_mul(l_op, r_op);

            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_min_mul(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);
                id_rule_min_mul(l_op, r_op);
            }
        }
        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
    
    stack_pop(stack);
    stack_pop(stack);
}

void rule_div(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);
    
    if((l_op->symb == e_num && r_op->symb == e_num))
    {
        // Celočíselné dělení
        l_op->symb = e_num;
    }
    else if((l_op->symb == e_dbl && r_op->symb == e_dbl))
    {
        // Desetinné dělení
        l_op->symb = e_dbl;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            check_e_id(l_op, sym_list);
            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                id_rule_div(l_op, r_op);

            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_div(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);
                id_rule_div(l_op, r_op);
            }
        }
        
        // TODO
        // musí být stejné typy, bez konverze (Int Int, Dbl Dbl, Str Str)
        // Pro typy s nil ? potřeba předtím operátor !
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
    
    if(strcmp(r_op->value, "0") == 0)
    {
        // fprintf(stderr, "ERROR: Cannot divide by 0 \n");
        error_caller(OTHER_ERROR);
        exit(OTHER_ERROR);
    }

    stack_pop(stack);
    stack_pop(stack);
}

void rule_rela(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);

    // if((l_op->symb == e_num && r_op->symb == e_num) || 
    //    (l_op->symb == e_dbl && r_op->symb == e_dbl) || 
    //    (l_op->symb == e_str && r_op->symb == e_str))
    if (((l_op->symb == e_num || l_op->symb == e_dbl) && 
        (r_op->symb == e_num || r_op->symb == e_dbl)) || (l_op->symb == e_str && r_op->symb == e_str))
    {
        // pretypovani r_op na int->double
        if (l_op->symb == e_dbl && r_op->symb == e_num)
        {
            r_op->symb = e_dbl;
        }
        // pretypovani l_op na int->double
        else if (l_op->symb == e_num && r_op->symb == e_dbl){
            l_op->symb = e_dbl;
        }

        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            check_e_id(l_op, sym_list);
            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                // pretypovani int->double
                if (l_op->symb == e_dbl && r_op->symb == e_num)
                {
                    r_op->symb = e_dbl;
                }

                id_rule_rela(l_op, r_op);

            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_rela(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);
                
                // pretypovani int->double
                if (l_op->symb == e_num && r_op->symb == e_dbl){
                    l_op->symb = e_dbl;
                }

                id_rule_rela(l_op, r_op);
            }
        }

        // TODO: potřeba Tabulky symbolů
        // můsí být stejné typy a bez nil (takže pro String? musí být před tím !)
        stack_pop(stack);
        stack_pop(stack);
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void rule_rela_equal(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);

    if(((l_op->symb == e_num || l_op->symb == e_dbl) && 
        (r_op->symb == e_num || r_op->symb == e_dbl)) || (l_op->symb == e_str && r_op->symb == e_str)
                                                      || (l_op->symb == e_nil && r_op->symb == e_nil))
    {
        // pretypovani r_op na int->double
        if (l_op->symb == e_dbl && r_op->symb == e_num)
        {
            r_op->symb = e_dbl;
        }
        // pretypovani l_op na int->double
        else if (l_op->symb == e_num && r_op->symb == e_dbl){
            l_op->symb = e_dbl;
        }

        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = e_bool;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            int l_type = check_e_id(l_op, sym_list);
            // printf("LLLLL%d\n", l_type);
            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                // pretypovani r_op na int->double
                if (l_op->symb == e_dbl && r_op->symb == e_num)
                {
                    r_op->symb = e_dbl;
                }


                // nil
                if ((l_type == TOKEN_TYPE_INT || l_type == TOKEN_TYPE_FLOAT || l_type == TOKEN_TYPE_STRING) && l_op->symb == e_nil)
                {
                    // printf("asdw\n");
                    // printf("AAAAAA%d\n", l_op->symb);
                    // printf("BBBBBB%d\n", r_op->symb);
                    if ((l_op->symb == e_nil && r_op->symb == e_nil) || 
                        (l_type == TOKEN_TYPE_INT && r_op->symb == e_num) || 
                        (l_type == TOKEN_TYPE_FLOAT && r_op->symb == e_dbl) ||
                        (l_type == TOKEN_TYPE_STRING && r_op->symb == e_str))
                    {
                        l_op->symb = e_bool;
                    } else {
                        error_caller(TYPE_COMP_ERROR);
                        exit(TYPE_COMP_ERROR);   
                    }
                }
                id_rule_rela_equal(l_op, r_op);
                
            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_rela_equal(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);

                // pretypovani l_op na int->double
                if (l_op->symb == e_num && r_op->symb == e_dbl){
                    l_op->symb = e_dbl;
                }

                id_rule_rela_equal(l_op, r_op);
            }
        }

        // TODO: potřeba Tabulky symbolů
        // můsí být také stejné typy (bez implicitní konverze) může být NIL type
        stack_pop(stack);
        stack_pop(stack);
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

// Nil coalescing operator
void rule_nil_coal(T_stack *stack, Tlist *sym_list)
{
    T_elem *l_op, *r_op;
    l_op = stack_get_val(stack, 2);
    r_op = stack_get_val(stack, 0);
    check_two_operands(*l_op, *r_op);
    
    if((l_op->symb == e_num && r_op->symb == e_num) ||
       (l_op->symb == e_dbl && r_op->symb == e_dbl) ||
       (l_op->symb == e_str && r_op->symb == e_str))
    {
        stack_pop(stack);
        stack_pop(stack);
        l_op->symb = r_op->symb;
    }
    else if(l_op->symb == e_id || r_op->symb == e_id || l_op->symb == e_id_exc || r_op->symb == e_id_exc)
    {
        if (l_op->symb == e_id || l_op->symb == e_id_exc){
            check_e_id(l_op, sym_list);
            // l_op = ID/ID!, r_op != ID/ID!
            if (r_op->symb != e_id && r_op->symb != e_id_exc)
            {
                id_rule_nil_coal(l_op, r_op);

            // l_op = ID/ID!, r_op = ID/ID!
            } else if(r_op->symb == e_id || r_op->symb == e_id_exc) {
                check_e_id(r_op, sym_list);
                id_rule_nil_coal(l_op, r_op);
            }
        } else if (l_op->symb != e_id && l_op->symb != e_id_exc){
            // l_op != ID/ID!, r_op = ID/ID!
            if (r_op->symb == e_id || r_op->symb == e_id_exc)
            {
                check_e_id(r_op, sym_list);
                id_rule_nil_coal(l_op, r_op);
            }
        }

        // TODO: potřeba Tabulky symbolů
        stack_pop(stack);
        stack_pop(stack);
        
    }
    else
    {
        // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
        error_caller(TYPE_COMP_ERROR);
        exit(TYPE_COMP_ERROR);
    }
}

void reduce_rule(T_stack *stack, T_elem *stack_top, Tlist *sym_list)
{
    // Podle terminálu na vrcholu zásobníku (expressions jsou přeskočeny) vybere pravidlo
    switch (stack_top->symb)
    {
    // Pravidla se dvěma operandy
    case prec_add:
        rule_plus(stack, sym_list);
        break;
    case prec_sub:
        rule_min_mul(stack, sym_list);
        //printf("- ");
        break;
    case prec_mul:
        rule_min_mul(stack, sym_list);
        //printf("* ");
        break;
    case prec_divi:
        //printf("/ ");
        rule_div(stack, sym_list);
        break;
        
    case prec_lt:
        //printf("< ");
        rule_rela(stack, sym_list);
        break;
    case prec_lt_eq:
        //printf("<= ");
        rule_rela(stack, sym_list);
        break;
    case prec_gt:
        //printf("> ");
        rule_rela(stack, sym_list);
        break;
    case prec_gt_eq:
        //printf(">= ");
        rule_rela(stack, sym_list);
        break;
    case prec_eq:
        //printf("== ");
        rule_rela_equal(stack, sym_list);
        break;
    case prec_n_eq:
        //printf("!= ");
        rule_rela_equal(stack, sym_list);
        break;
    
    // Pravidlo E->id!
    case prec_exc:
        stack_top->symb = e_id_exc;
        break;

    /* Vyhodnocení závorky stav zásobníku: ), EXPRESSION, (
       potřeba popnout závorky ze zásobníku a zároveň ponechat EXPRESSION*/
    case prec_r_brac:;
        T_elem *op = stack_get_val(stack, 1);
        T_token token = op->token;
        prec_symb symbol = op->symb;
        token.value = malloc(strlen(op->value)+2);
        strcpy(token.value, op->value);

        stack_pop(stack);
        stack_pop(stack);
        stack_pop(stack);
        stack_push(stack, token, token.value, symbol);      // Navrácení Expression zpět na zásobník
        free(token.value);
        break;
    
    case prec_que:
        //printf("??");
        rule_nil_coal(stack, sym_list);
        break;
    
    // Pravidla typu: E->id, E->int atd, jsou vyřešeny změnou indexu tokenu (stack_top->symb) na vrcholu zásobníku
    case prec_id:
        stack_top->symb = e_id;
        //printf("%s, ", stack_top->value);
        break;
    case prec_num:
        stack_top->symb = e_num;
        //printf("%s, ", stack_top->value);
        break;
    case prec_dbl:
        stack_top->symb = e_dbl;
        //printf("%s, ", stack_top->value);
        break;
    case prec_str:
        stack_top->symb = e_str;
        //printf("%s, ", stack_top->value);
        break;
    case prec_nil:
        stack_top->symb = e_nil;
        //printf("%s, ", nil);
        break;
    
    // Ukončovací symbol
    case prec_end:
        stack_top->symb = e_end;
        break;
    
    default:
        // fprintf(stderr, "ERROR: Syntax error\n");
        error_caller(SYN_ERROR);
        exit(SYN_ERROR);
    }
}



T_token_type expr_parser(FILE* file, T_queue *queue, Tlist *sym_list)
{

// Precedenční tabulka:
const char preced_tab [20][20] = {
/* +    -    *    /   id!   (    )    <   <=    >   >=   ==   !=    id  int  dbl  str  nil  ??    $ - token*/
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', '<', 'x', '>', '>'},// +
{ '>', '>', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', 'x', '>', '>'},// -
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', 'x', '>', '>'},// *
{ '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '<', 'x', 'x', '>', '>'},// /
{ '>', '>', '>', '>', 'x', '<', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// id!
{ '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'x'},// (
{ '>', '>', '>', '>', 'x', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// )
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', 'x', '>'},// <
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', 'x', '>'},// <=
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', 'x', '>'},// >
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', 'x', 'x', '>'},// >=
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', '<', 'x', '>'},// ==
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', '<', 'x', '>'},// !=
{ '>', '>', '>', '>', 'x', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// id
{ '>', '>', '>', '>', 'x', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// int
{ '>', '>', '>', '>', 'x', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// dbl
{ '>', 'x', 'x', 'x', 'x', 'x', '>', '>', '>', '>', '>', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// str
{ 'x', 'x', 'x', 'x', 'x', 'x', '>', 'x', 'x', 'x', 'x', '>', '>', 'x', 'x', 'x', 'x', 'x', '>', '>'},// nil
{ '<', '<', '<', '<', '<', '<', '>', 'x', 'x', 'x', 'x', 'x', 'x', '<', '<', '<', '<', '<', '<', '>'},// ??
{ '<', '<', '<', '<', '<', '<', 'x', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'e'} // $
};
    int end = 0;                        // Příznak ukončení parsování výrazů
    int end_expr = 0;                   // Příznak nalezení nestandardního ukončení výrazu
    T_stack *stack = stack_init();      // Inicializace zásobníku
    T_token init_tok;
    init_tok.type = TOKEN_START_PREC;
    init_tok.valueLength = 1;

    // Na vrchol zásobníku vložím počáteční symbol $
    stack_push(stack, init_tok, "x", prec_end);
    T_elem *stack_top = stack_get_val(stack, 0);
    prec_symb result;
    
    T_token token = getToken(queue, file);
    while(!end)
    {
        // Získání prvního terminálu ze zásobníku (přeskočení expressions)
        int idx = 0;
        stack_top = stack_get_val(stack, idx);
            /* Pokud je prvek na vrcholu expression (již vyhodnocená část výrazu), 
            přeskoč ho a podívej se na další prvek*/
            while(stack_top->symb > expressions)
                stack_top = stack_get_val(stack, ++idx);

        // Získání indexů pro tabulku
        int idx_col, idx_row;
        if(end_expr)
            // Pokud bylo nalezeno nestandardní ukončení výrazu, není potřeba volat funcki pro získání indexu
            idx_col = prec_end;
        else
            idx_col = get_prec_value(token, &end_expr, queue, file);
        
        idx_row = stack_top->symb;

        switch (preced_tab[idx_row][idx_col])
        {
        // Operace shift -> přidej token na vrchol zásobníku a načti další
        case '<':
            stack_push(stack, token, token.value, idx_col);
            if(token.valueLength)
                free(token.value);
            token = getToken(queue, file);
            break;

        // Proveď redukci a aplikuj pravidlo
        case '>':
            reduce_rule(stack, stack_top, sym_list);
            break;

        // Pro vyhodnocení závorek -> je to vlastně operace shift a zároveň redukce
        case '=':
            stack_push(stack, token, token.value, idx_col);
            stack_top = stack_get_val(stack, 0);
            reduce_rule(stack, stack_top, sym_list);
            if(token.valueLength)
                free(token.value);
            token = getToken(queue, file);
            break;

        // Chyba podle precedenční tabulky
        case 'x':
            // Např. výraz "string" * 7, je Sémantická chyba, ne Syntaktická
            if((idx_col == prec_str && (idx_row == prec_sub || idx_row == prec_mul || idx_row == prec_divi)) ||
               (idx_row == prec_str && (idx_col == prec_sub || idx_col == prec_mul || idx_col == prec_divi)))
            {
                // fprintf(stderr, "ERROR: Expression error, invalid data types\n");
                error_caller(TYPE_COMP_ERROR);
                exit(TYPE_COMP_ERROR);
            }
            else
            {
                // fprintf(stderr, "ERROR: Syntax error, prec table fail\n");
                error_caller(SYN_ERROR);
                exit(SYN_ERROR);
            }
        
        // Konec precedeneční analýzy
        case 'e':
            end = 1;
            stack_top = stack_get_val(stack, 0);
            result = stack_top->symb;
            // Pokud byl výraz ukončen standardně, vlož poslední token, který již není součástí výrazu, do fronty
            if(!end_expr)
                queue_add(queue, token);
            break;
        }
    }

    // pokud prijde jenom id do vyrazu
    if (result == e_id || result == e_id_exc){
        check_e_id(stack->top, sym_list);
        result = stack->top->symb;
    }

    stack_empty(stack);

    T_token_type token_res;
    switch (result)
    {
    case e_num:
        token_res = TOKEN_KW_INT;
        break;
    case e_dbl:
        token_res = TOKEN_KW_DOUBLE;
        break;
    case e_str:
        token_res = TOKEN_KW_STRING;
        break;
    case e_nil:
        token_res = TOKEN_KW_NIL;
        break;
    case e_bool:
        token_res = TOKEN_BOOL;
        break;  
    default:
        token_res = TOKEN_VOID;
        break;
    }
    //  printf("rrrr%d\n", token_res);
    return token_res;
}
