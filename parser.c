/*
IFJ Projekt 2023

PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "parser.h"
#define UNUSED(x) (void)(x)

// <prog> -> <st-list> 
bool prog(T_token token, T_queue *queue, FILE *file) {
    token = getToken(queue, file);
    return st_list(token, queue, file);
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token, T_queue *queue, FILE *file) {
    // EOF
    if (token.type == TOKEN_EOF) {
        return true;
    } else {
        // <stat>
        token = getToken(queue, file);
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
        // <id-type>
        token = getToken(queue, file);
        if(id_type(token, queue, file)) return true;
        
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
    UNUSED(queue);
    UNUSED(token);

    expr_parser(file);
    // tady by mel expr_parser vlozit to co nemel precist zpatky sam
    return true;
}

//_____

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
        default:
            return false;
            break;
    }
    return false;
}

//<id-type> -> : type <assign>
//<id-type> -> = <exp>
bool id_type(T_token token, T_queue *queue, FILE *file){
    T_token tmp = getToken(queue, file);
    if(token.type == TOKEN_COLON && IsType(tmp)){
        T_token next_token = getToken(queue, file);
        return assign(next_token, queue, file);
    }else if(token.type == TOKEN_ASSIGN){
        expr_parser(file);
        return true;
        
    }else{  
        return false;
    }
    return false;
}

// <assign> -> = <call>
// <assign> -> eps
bool assign(T_token token, T_queue *queue, FILE *file){
    if(token.type == TOKEN_ASSIGN){
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    }else{//moze tam byt hocico
        queue_add(queue, token);
        return true;
    }
    return false;
}

//<id-stat> -> = <call>
//<id-stat> –> ( <term-list> )
bool id_stat(T_token token, T_queue *queue, FILE *file){
    if(token.type == TOKEN_ASSIGN){
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    }else if(token.type == TOKEN_L_RND){
        T_token next_token = getToken(queue, file);
        if(term_list(next_token, queue, file)){
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
    if(token.type == TOKEN_ID){
        T_token tmp = getToken(queue, file);
        if( tmp.type == TOKEN_L_RND){
            T_token next_token = getToken(queue, file);
            if(term_list(next_token, queue, file)){
                T_token final_token = getToken(queue, file);
                if(final_token.type == TOKEN_R_RND){
                    return true;
                }
            }
        }
        return false;
    //}else if(expr_parser(file)){
      //  return true;
    }else{
        expr_parser(file);
        return true;
    }
    return false;
}

// <exp-stat> -> <exp>
// <exp-stat> -> let id
bool exp_stat(T_token token, T_queue *queue, FILE *file){
    if(token.type == TOKEN_KW_LET){
        T_token tmp = getToken(queue, file);
        if(tmp.type == TOKEN_ID){
            return true;
        }
        return false;
    }else{
        expr_parser(file);
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
        queue_add(queue, token);
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

// <term> -> id
// <term> -> lit
// <term> -> p-name : val 
bool term(T_token token, T_queue *queue, FILE *file){
    // id
    if (token.type == TOKEN_ID) {
        return true;
    // p-name
    } else if (token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_UNDERSCORE){
        // :
        token = getToken(queue, file);
        if (token.type == TOKEN_COLON) {
            // value
            token = getToken(queue, file);
            if(token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID) {
                return true;
            }
        }
        return false;
    // num
    } else if (token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM) {
        return true;
    } else {
        return false;
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
        if (token.type == TOKEN_KW_DOUBLE || token.type == TOKEN_KW_INT || token.type == TOKEN_KW_STRING) // || token.type == TOKEN_STRING_NIL || token.type == TOKEN_INT_NIL || token.type == TOKEN_DOUBLE_NIL)
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
    // p-name
    if (token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_UNDERSCORE)
    {
        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)// || token.type == TOKEN_ID_NIL)
        {
            // :
            token = getToken(queue, file);
            if (token.type == TOKEN_COLON)
            {
                // type
                token = getToken(queue, file);
                if (token.type == TOKEN_KW_DOUBLE || token.type == TOKEN_KW_STRING || token.type == TOKEN_KW_INT )//|| token.type == TOKEN_STRING_NIL || token.type == TOKEN_INT_NIL || token.type == TOKEN_DOUBLE_NIL)
                {
                    return true;
                }
            }
        }        
    }
    return false;
}
