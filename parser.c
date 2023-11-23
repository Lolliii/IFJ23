/*
IFJ Projekt 2023

PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "parser.h"

bool IsTerm(T_token token){
    switch(token.type){
        case TOKEN_INT:
            return true;
            break;
        case TOKEN_INT_EXP:
            return true;
            break;
        case TOKEN_INT_EXP_PM:
            return true;
            break;
        case TOKEN_DOUBLE:
            return true;
            break;
        case TOKEN_DOUBLE_EXP:
            return true;
            break;
        case TOKEN_DOUBLE_EXP_PM:
            return true;
            break;
        case TOKEN_STRING:
            return true;
            break;
        case TOKEN_ML_STRING:
            return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}

bool IsType(T_token token){
    switch(token.type){
        case TOKEN_TYPE_INT:
            return true;
            break;
        case TOKEN_TYPE_FLOAT:
            return true;
            break;
        case TOKEN_TYPE_STRING:
            return true;
            break;
        case TOKEN_KW_INT:
            return true;
            break;
        case TOKEN_KW_DOUBLE:
            return true;
            break;
        case TOKEN_KW_STRING:
            return true;
            break;
        default:
            return false;
            break;
    }
    return false;
}

// <prog> -> <st-list> 
bool prog(T_token token, T_queue *queue, FILE *file) {
    if(!st_list(token, queue, file)){
        error_caller(SYN_ERROR);
        exit(SYN_ERROR);
    }
    return true;
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token, T_queue *queue, FILE *file) {
    // EOF
    if (token.type == TOKEN_EOF) {
        return true;
    } else {
        // <stat>
        if (stat(token, queue, file))
        {
            // <st-list>
            token = getToken(queue, file);
            if (st_list(token, queue, file)) return true;
        }
    }
    return false;
}

// <body> -> <stat> <body>
// <body> -> eps
bool body(T_token token, T_queue *queue, FILE *file) {
    // <stat>
    if (stat(token, queue, file))
    {
        // <body>
        token = getToken(queue, file);
        if (body(token, queue, file))
        {
            return true;
        }
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <stat> -> while <exp-stat> { <body> }                  
// <stat> -> if <exp-stat> { <body> } else { <body> }            
// <stat> -> func id ( <param-list> ) <f-type> { <body> }       
// <stat> -> let id <id-type>                                  
// <stat> -> var id <id-type>                                    
// <stat> -> id <id-stat>   
// <stat> -> return <ret-stat> 
bool stat(T_token token, T_queue *queue, FILE *file) {
    // <stat> -> while <exp-stat> { <body> }
    if (token.type == TOKEN_KW_WHILE)
    {
        // <exp-stat>
        token = getToken(queue, file);
        if (exp_stat(token, queue, file))
        {
            // {
            token = getToken(queue, file);
            if (token.type == TOKEN_L_CURL)
            {
                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    
    // <stat> -> if <exp-stat> { <body> } else { <body> }
    } else if (token.type == TOKEN_KW_IF) {
        // <exp-stat>
        token = getToken(queue, file);
        if (exp_stat(token, queue, file))
        {
            // {
            token = getToken(queue, file);
            if (token.type == TOKEN_L_CURL)
            {
                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
                        // else
                        token = getToken(queue, file);
                        if (token.type == TOKEN_KW_ELSE)
                        {
                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
    
    // <stat> -> func id ( <param-list> ) <f-type> { <body> }
    } else if (token.type == TOKEN_KW_FUNC) {
        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // (
            token = getToken(queue, file);
            if (token.type == TOKEN_L_RND)
            {
                // <param-list>
                token = getToken(queue, file);
                if (param_list(token, queue, file))
                {
                    // )
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_RND)
                    {
                        // <f-type>
                        token = getToken(queue, file);
                        if (f_type(token, queue, file))
                        {
                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return false;
        
    // <stat> -> let id <id-type>
    } else if (token.type == TOKEN_KW_LET) {
        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file)) return true;
        }
        return false;
        
    // <stat> -> var id <id-type>
    } else if (token.type == TOKEN_KW_VAR) {
        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file)) return true;
        }
        return false;
        
    // <stat> -> id <id-stat>
    } else if (token.type == TOKEN_ID) {
        // <id-stat>
        token = getToken(queue, file);
        if(id_stat(token, queue, file)) return true;
        
        return false;

    // <stat> -> return <ret-stat>
    } else if (token.type == TOKEN_KW_RETURN) {
        // <ret_stat>
        token = getToken(queue, file);
        if(ret_stat(token, queue, file)) return true;

        return false;
    }
    return false;
}

// <ret-stat> -> <exp>
// <ret-stat> -> eps
bool ret_stat(T_token token, T_queue *queue, FILE *file) {
    // <exp>
    if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_R_CURL || token.type == TOKEN_L_RND)
    {
        queue_add(queue, token);
    }
    expr_parser(file, queue);
    
    return true;
}

//<id-type> -> : type <assign>
//<id-type> -> = <call>
bool id_type(T_token token, T_queue *queue, FILE *file){
    // :    
    if(token.type == TOKEN_COLON)
    {
        // type
        T_token tmp = getToken(queue, file);
        if (IsType(tmp))
        {
            // <assign>
            T_token next_token = getToken(queue, file);
            return assign(next_token, queue, file);
        }
    // =
    }else if(token.type == TOKEN_ASSIGN){
        // <call>
        token = getToken(queue, file);
        return call(token, queue, file);
    }else{
        return false;
    }
    return false;
}

// <assign> -> = <call>
// <assign> -> eps
bool assign(T_token token, T_queue *queue, FILE *file){
    // =
    if(token.type == TOKEN_ASSIGN){
        // <call>
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    // eps
    }else{
        queue_add(queue, token);
        return true;
    }
    return false;
}

//<id-stat> -> = <call>
//<id-stat> –> ( <term-list> )
bool id_stat(T_token token, T_queue *queue, FILE *file){
    // =
    if(token.type == TOKEN_ASSIGN){
        // <call>
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    // (
    }else if(token.type == TOKEN_L_RND){
        // <term-list>
        T_token next_token = getToken(queue, file);
        if(term_list(next_token, queue, file)){
            // )
            token = getToken(queue, file);
            if(token.type == TOKEN_R_RND){
                return true;
            }
        }
        return false;
    }else{
        return false;
    }
    return false;
}

// <call> -> id ( <term-list> )
// <call> -> <exp>
bool call(T_token token, T_queue *queue, FILE *file){
    // id
    if(token.type == TOKEN_ID){
        // kouknu dopredu na dalsi token
        T_token tmp = getToken(queue, file);
        // id ( --> volani funkce
        if (tmp.type == TOKEN_L_RND)
        {
            token = tmp;
            // <term-list>
            token = getToken(queue, file);
            if(term_list(token, queue, file))
            {
                // )
                token = getToken(queue, file);
                if(token.type == TOKEN_R_RND){
                    return true;
                }
            }
            return false;

        // neni to volani funkce --> je to vyraz
        // id <exp>
        } else {
            queue_add(queue, token);
            queue_add(queue, tmp);

            expr_parser(file, queue);
            return true;
        }
        return false;
    // <exp>
    }else{
        if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
        }
        expr_parser(file, queue);
        return true;
    }
    return false;
}

// <exp-stat> -> <exp>
// <exp-stat> -> let id
bool exp_stat(T_token token, T_queue *queue, FILE *file){
    // let
    if(token.type == TOKEN_KW_LET){
        // id
        T_token tmp = getToken(queue, file);
        if(tmp.type == TOKEN_ID){
            return true;
        }
        return false;
    // <exp>
    }else{
        if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
        }
        
        expr_parser(file, queue);
        return true;
    }
    return false;
}

// <term-list> -> <term> <t-list>
// <term-list> -> eps
bool term_list(T_token token, T_queue *queue, FILE *file) {
    // <term>
    if (term(token, queue, file))
    {
        // <t-list>
        token = getToken(queue, file);
        return t_list(token, queue, file);
    // eps
    } else {
        if (token.type == TOKEN_R_RND)
        {
            queue_add(queue, token);
        }
        return true;
    }
    return false;
}

// <t-list> -> , <term> <t-list>
// <t-list> -> eps 
bool t_list(T_token token, T_queue *queue, FILE *file) {
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <term>
        token = getToken(queue, file);
        if (term(token, queue, file)) {
            // <t-list>
            token = getToken(queue, file);
            return t_list(token, queue, file);
        }
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <term> -> lit
// <term> -> id <term-name>
bool term(T_token token, T_queue *queue, FILE *file){
    // id
    if (token.type == TOKEN_ID) {
        // <term-name>
        token = getToken(queue, file);
        return term_name(token, queue, file);

    // lit
    } else if (IsTerm(token) || token.type == TOKEN_KW_NIL) {
        return true;
    } else {
        return false;
    }
    return false;
}

// <term-name> -> : val
// <term-name> -> eps 
bool term_name(T_token token, T_queue *queue, FILE *file) {
    // :
    if (token.type == TOKEN_COLON)
    {
        // value
        token = getToken(queue, file);
        if(IsTerm(token) || token.type == TOKEN_ID) {
            return true;
        }
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <f-type> -> -> type
// <f-type> -> eps  
bool f_type(T_token token, T_queue *queue, FILE *file) {
    // ->
    if (token.type == TOKEN_FUNCTION_TYPE)
    {
        // type - Double, Int, String, ?
        token = getToken(queue, file);
        if (IsType(token))
        {
            return true;
        }
        return false;
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }   
    return false;
}

// <param-list> -> <param> <p-list>
// <param-list> -> eps
bool param_list(T_token token, T_queue *queue, FILE *file) {
    // <param>
    if (param(token, queue, file))
    {
        // <p-list>
        token = getToken(queue, file);
        return p_list(token, queue, file);
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <p-list> -> , <param> <p-list>
// <p-list> -> eps
bool p_list(T_token token, T_queue *queue, FILE *file){
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <param>
        token = getToken(queue, file);
        if (param(token, queue, file))
        {
            // <p-list>
            token = getToken(queue, file);
            return p_list(token, queue, file);
        }
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <param> -> p-name id : type
bool param(T_token token, T_queue *queue, FILE *file){
    // p-name, _
    if (token.type == TOKEN_ID || token.type == TOKEN_UNDERSCORE)
    {
        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // :
            token = getToken(queue, file);
            if (token.type == TOKEN_COLON)
            {
                // type - Double, Int, String, ?
                token = getToken(queue, file);
                if (IsType(token))
                {
                    return true;
                }
            }
        }        
    }
    return false;
}
