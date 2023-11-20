/*
IFJ Projekt 2023

PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "parser.h"

// <prog> -> <st-list> 
bool prog(T_token token) {
    st_list();
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token) {
    // <stat>
    if (stat())
    {
        // <st-list>
        if (st_list())
        {
            return true;
        }
    // EOF    
    }else if (token.type == TOKEN_EOF) {
        return true;
    }
    return false;
}

// <body> -> <stat> <body>
// <body> -> eps
bool body(T_token token) {
    // <stat>
    if (stat())
    {
        // <body>
        if (body())
        {
            return true;
        }
    // eps
    } else {
        return true;
    }
    return false;
}


bool stat(T_token token) {
    // <stat> -> while <exp-stat> { <body> }
    if (token.type == TOKEN_KW_WHILE)
    {
        // <exp-stat>
        if (exp_stat())
        {
            // {
            if (getNextToken() == TOKEN_L_CURL)
            {
                // <body>
                if (body())
                {
                    // }
                    if (getNextToken() == TOKEN_R_CURL)
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
        if (exp_stat())
        {
            // {
            if (getNextToken() == TOKEN_L_CURL)
            {
                // <body>
                if (body())
                {
                    // }
                    if (getNextToken() == TOKEN_R_CURL)
                    {
                        // else
                        if (getNextToken() == TOKEN_KW_ELSE)
                        {
                            // {
                            if (getNextToken() == TOKEN_L_CURL)
                            {
                                // <body>
                                if (body())
                                {
                                    // }
                                    if (getNextToken() == TOKEN_R_CURL)
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
        if (token.type == TOKEN_ID)
        {
            // (
            if (getNextToken() == TOKEN_L_RND)
            {
                // <param-list>
                if (param_list())
                {
                    // )
                    if (getNextToken() == TOKEN_R_RND)
                    {
                        // <f-type>
                        if (f_type())
                        {
                            // {
                            if (getNextToken() == TOKEN_L_CURL)
                            {
                                // <body>
                                if (body())
                                {
                                    // }
                                    if (getNextToken() == TOKEN_R_CURL)
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
        if (getNextToken() == TOKEN_ID)
        {
            // <id-type>
            id_type();
        }
        return false;
        
    // <stat> -> var id <id-type>
    } else if (token.type == TOKEN_KW_VAR) {
        // id
        if (getNextToken() == TOKEN_ID)
        {
            // <id-type>
            id_type();
        }
        return false;
        
    // <stat> -> id <id-stat>
    } else if (token.type == TOKEN_ID) {
        // <id-stat>
        id_stat();

    // <stat> -> return <ret-stat>
    } else if (token.type == TOKEN_KW_RETURN) {
        ret_stat();
    }
    return false;
}

// <ret-stat> -> <exp>
// <ret-stat> -> eps
                                                        // ???????
bool ret_stat(T_token token) {
    if (token.type)
    {
        /* code */
    }
    
}

// <id-type> -> : type <assign>
// <id-type> -> = <exp>
bool id_type(T_token token) {
    // :
    if (token.type == TOKEN_COLON)
    {
        // type - Double, String, Int
        if (getNextToken() == TOKEN_KW_DOUBLE || getNextToken() == TOKEN_KW_STRING || getNextToken() == TOKEN_KW_INT) {
            // <assign>
            assign();
        }
    // =
    } else if (token.type == TOKEN_ASSIGN) {
        // call expr parser
    }
    return false;
}

// <assign> -> = <call>
// assign> -> eps
bool assign(T_token token) {
    // =
    if (token.type == TOKEN_ASSIGN)
    {
        // <call>
        call();
    // eps
    } else {
        return true;
    }
}

// <id-stat> –> ( <term-list> ) 
// <id-stat> -> = <call>
bool id_stat(T_token token) {
    // (
    if (token.type == TOKEN_L_RND)
    {
        // <term-list>
        if (term_list())
        {
            // )
            if (getNextToken() == TOKEN_R_RND){
                return true;
            }
        }
    // =
    } else if (token.type == TOKEN_ASSIGN) {
        // <call>
        call();
    }
}

// <call> -> id ( <term-list> )
// <call> -> <exp>
bool call(T_token token) {
    // id
    if (token.type == TOKEN_ID)
    {
        // (
        if (getNextToken() == TOKEN_L_RND)
        {
            // <term-list>
            if (term_list())
            {
                // )
                if (getNextToken() == TOKEN_R_RND){
                    return true;
                }
            }
        }
        return false;        
    // <exp>
    } else {
        // call expr parser
    }
}

// <exp-stat> -> let id
// <exp-stat> -> <exp>
bool exp_stat(T_token token) {
    // let
    if (token.type == TOKEN_KW_LET)
    {
        // id
        if (getNextToken() == TOKEN_ID)
        {
            return true;
        }
        return false;
    // <exp>
    } else {
        // call expr parser
    }
}

// <term-list> -> <term> <t-list>
// <term-list> -> eps
bool term_list(T_token token) {
    // <term>
    if (term())
    {
        // <t-list>
        t_list();
    // eps
    } else {
        return true;
    }
}

// <t-list> -> , <term> <t-list>
// <t-list> -> eps 
bool t_list(T_token token) {
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <term>
        if (term()) {
            // <t-list>
            t_list();
        }
    // eps
    } else {
        return true;
    }
}

// <term> -> id
// <term> -> lit
// <term> -> p-name : val 
bool term(T_token token){
    // id
    if (token.type == TOKEN_ID) {
        return true;
    // p-name
    } else if (token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING) {
        // :
        if (getNextToken() == TOKEN_COLON) {

            // value
            if(getNextToken() == token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID) {
                return true;
            }
        }
        return false;
    // num
    } else if (token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM) {
        return true;
    }
    return true;
}

// <f-type> -> -> type
// <f-type> -> eps  
bool f_type(T_token token) {
    // ->
    if (token.type == TOKEN_FUNCTION_TYPE)
    {
        // type - Double, Int, String
        if (getNextToken() == TOKEN_KW_DOUBLE || getNextToken() == TOKEN_KW_STRING || getNextToken() == TOKEN_KW_INT)
        {
            return true;
        }
    // eps
    } else {
        return true;
    }   
}

// <param-list> -> <param> <p-list>
// <param-list> -> eps
bool param_list(T_token token) {
    // <param>
    if (param())
    {
        // <p-list>
        p_list();
    // eps
    } else {
        return true;
    }
}

// <p-list> -> , <param> <p-list>
// <p-list> -> eps
bool p_list(T_token token){
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <param>
        if (param()) {
            // <p-list>
            p_list();
        }
    // eps
    } else {
        return true;
    }
}

// <param> -> p-name id : type
bool param(T_token token){
    // p-name
    if (token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING)
    {
        return true;
    }
    return false;
}