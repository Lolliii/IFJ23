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
bool prog(T_token token, T_queue *queue, FILE *file) {
    printf("<prog>\n");

    //token = getToken(queue, file);
    return st_list(token, queue, file);
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token, T_queue *queue, FILE *file) {
    // EOF
    printf("<st_list>\n");
    if (token.type == TOKEN_EOF) {
        printf("eof\n");
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
//    printf("end st-list\n");
    return false;
}

// <body> -> <stat> <body>
// <body> -> eps
bool body(T_token token, T_queue *queue, FILE *file) {
    // <stat>
    printf("<body>\n");
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
        printf("eps\n");

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
    printf("<stat>\n");

    if (token.type == TOKEN_KW_WHILE)
    {
        printf("<while>\n");

        // <exp-stat>
        token = getToken(queue, file);
        if (exp_stat(token, queue, file))
        {
            // {
            token = getToken(queue, file);
            if (token.type == TOKEN_L_CURL)
            {
            printf("{\n");

                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
            printf("}\n");

                        return true;
                    }
                }
            }
        }
        return false;
    
    // <stat> -> if <exp-stat> { <body> } else { <body> }
    } else if (token.type == TOKEN_KW_IF) {
        printf("<if>\n");

        // <exp-stat>
        token = getToken(queue, file);
        if (exp_stat(token, queue, file))
        {


            // {
            token = getToken(queue, file);

            if (token.type == TOKEN_L_CURL)
            {
            printf("{\n");

                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
            printf("}\n");

                        // else
                        token = getToken(queue, file);
                        if (token.type == TOKEN_KW_ELSE)
                        {
            printf("else\n");

                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
            printf("{\n");

                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
            printf("}\n");

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
        printf("<func>\n");

        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            printf("id\n");

            // (
            token = getToken(queue, file);
            if (token.type == TOKEN_L_RND)
            {
                printf("(\n");

                // <param-list>
                token = getToken(queue, file);
                if (param_list(token, queue, file))
                {
                    // )
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_RND)
                    {
                        printf(")\n");

                        // <f-type>
                        token = getToken(queue, file);
                        if (f_type(token, queue, file))
                        {
                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
                                printf("{\n");

                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
                                        printf("}\n");
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
        printf("let\n");

        // id
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            printf("id\n");

            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file)) return true;
        }
        return false;
        
    // <stat> -> var id <id-type>
    } else if (token.type == TOKEN_KW_VAR) {
        printf("var\n");


        // id
        token = getToken(queue, file);


        if (token.type == TOKEN_ID)
        {
        printf("id\n");

            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file)) return true;
        }
        return false;
        
    // <stat> -> id <id-stat>
    } else if (token.type == TOKEN_ID) {
        printf("id\n");

        // <id-stat>
        token = getToken(queue, file);
        if(id_stat(token, queue, file)) return true;
        
        return false;
    // <stat> -> return <ret-stat>
    } else if (token.type == TOKEN_KW_RETURN) {
        printf("return\n");

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
    printf("<ret_stat>\n");

    printf("<expr>\n");
    if (token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_R_CURL || token.type == TOKEN_L_RND)
    {
        queue_add(queue, token);
    }

    expr_parser(file, queue);
    printf("<expr good>\n");
    return true;
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

//<id-type> -> : type <assign>
//<id-type> -> = <call>
bool id_type(T_token token, T_queue *queue, FILE *file){
    printf("<id_type>\n");
    
    if(token.type == TOKEN_COLON)
    {
        printf(":\n");
        T_token tmp = getToken(queue, file);
        if (IsType(tmp))
        {
            printf("type\n");
            T_token next_token = getToken(queue, file);
            return assign(next_token, queue, file);
        }
        
    }else if(token.type == TOKEN_ASSIGN){
        printf("=\n");

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
    printf("<assign>\n");
    if(token.type == TOKEN_ASSIGN){
        printf("=\n");

        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    }else{//moze tam byt hocico
        printf("eps\n");

        queue_add(queue, token);
        return true;
    }
    return false;
}

//<id-stat> -> = <call>
//<id-stat> –> ( <term-list> )
bool id_stat(T_token token, T_queue *queue, FILE *file){
    printf("<id_stat>\n");
    // =
    if(token.type == TOKEN_ASSIGN){
        printf("=\n");

        // <call>
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file);
    // (
    }else if(token.type == TOKEN_L_RND){
        printf("(\n");

        // <term-list>
        T_token next_token = getToken(queue, file);
        if(term_list(next_token, queue, file)){
            // )
            token = getToken(queue, file);
            if(token.type == TOKEN_R_RND){
                printf(")\n");

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
    printf("<call>\n");
    // id
    if(token.type == TOKEN_ID){
        printf("id\n");
        T_token tmp = getToken(queue, file);

        // id (
        // kouknu na dopredu na dalsi token
        if (tmp.type == TOKEN_L_RND)
        {
            token = tmp;
            printf("(\n");

            // <term-list>
            token = getToken(queue, file);
            if(term_list(token, queue, file))
            {
                // )
                token = getToken(queue, file);
                if(token.type == TOKEN_R_RND){
                    printf(")\n");

                    return true;
                }
            }
            return false;

        // neni to volani funkce --> je to vyraz
        // id <exp>
        } else {
            queue_add(queue, token);
            queue_add(queue, tmp);
            
            printf("<expr>\n");
            expr_parser(file, queue);
            printf("<expr good>\n");

            return true;
        }
        return false;
    // <exp>
    }else{
        printf("<exp>\n");

        if (token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);

        }

// token = getToken(queue, file);
// printTokenName(token);
// printf("\n");

// token = getToken(queue, file);
// printTokenName(token);
// printf("\n");

// token = getToken(queue, file);
// printTokenName(token);
// printf("\n");

// token = getToken(queue, file);
// printTokenName(token);
// printf("\n");

// token = getToken(queue, file);
// printTokenName(token);
// printf("\n");



        expr_parser(file, queue);
        printf("<expr good>\n");

        return true;
    }
    return false;
}

// <exp-stat> -> <exp>
// <exp-stat> -> let id
bool exp_stat(T_token token, T_queue *queue, FILE *file){
    printf("<exp_stat>\n");
    if(token.type == TOKEN_KW_LET){
        printf("let\n");

        T_token tmp = getToken(queue, file);
        if(tmp.type == TOKEN_ID){
            printf("id\n");

            return true;
        }
        return false;
    }else{
        printf("<exp>\n");

        if (token.type == TOKEN_L_RND || token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
        }
        
        expr_parser(file, queue);
        printf("<expr good>\n");

        return true;
    }
    return false;
}

// <term-list> -> <term> <t-list>
// <term-list> -> eps
bool term_list(T_token token, T_queue *queue, FILE *file) {
    printf("<term_list>\n");
    // <term>
    if (term(token, queue, file))
    {
        printf("<t-list>\n");

        // <t-list>
        token = getToken(queue, file);
        return t_list(token, queue, file);
    // eps
    } else {
        printf("eps\n");

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
    printf("<t_list>\n");
    // ,
    if (token.type == TOKEN_COMMA)
    {
        printf(",\n");

        // <term>
        token = getToken(queue, file);
        if (term(token, queue, file)) {
            printf("<t-list\n");

            // <t-list>
            token = getToken(queue, file);
            return t_list(token, queue, file);
        }
    // eps
    } else {
        printf("eps\n");

        queue_add(queue, token);
        return true;
    }
    return false;
}

// <term> -> lit
// <term> -> id <term-name>
bool term(T_token token, T_queue *queue, FILE *file){
    printf("<term>\n");
    // id
    if (token.type == TOKEN_ID) {
        printf("id\n");

        // <term-name>
        token = getToken(queue, file);
        return term_name(token, queue, file);

    // lit
    } else if (token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_KW_NIL) {
        printf("lit\n");
        return true;
    } else {
        return false;
    }
    return false;
}

// <term-name> -> : val
// <term-name> -> eps 
bool term_name(T_token token, T_queue *queue, FILE *file) {
    printf("<term-name>\n");

    // :
    if (token.type == TOKEN_COLON)
    {
        printf(":\n");

        // value
        token = getToken(queue, file);
        if(token.type == TOKEN_INT || token.type == TOKEN_INT_EXP || token.type == TOKEN_INT_EXP_PM || token.type == TOKEN_DOUBLE || token.type == TOKEN_DOUBLE_EXP || token.type == TOKEN_DOUBLE_EXP_PM || token.type == TOKEN_STRING || token.type == TOKEN_ML_STRING || token.type == TOKEN_ID) {
            printf("value\n");
            return true;
        }
    // eps
    } else {
        printf("eps\n");
        queue_add(queue, token);
        return true;
    }
    
    return false;
}

// <f-type> -> -> type
// <f-type> -> eps  
bool f_type(T_token token, T_queue *queue, FILE *file) {
    printf("<f_type>\n");
    // ->
    if (token.type == TOKEN_FUNCTION_TYPE)
    {
        printf("->\n");

        // type - Double, Int, String, ?
        token = getToken(queue, file);
        if (token.type == TOKEN_KW_DOUBLE || token.type == TOKEN_KW_INT || token.type == TOKEN_KW_STRING || token.type == TOKEN_TYPE_FLOAT || token.type == TOKEN_TYPE_INT || token.type == TOKEN_TYPE_STRING)
        {
            printf("type\n");

            return true;
        }
        return false;
    // eps
    } else {
        printf("eps\n");

        queue_add(queue, token);
        return true;
    }   
    return false;
}

// <param-list> -> <param> <p-list>
// <param-list> -> eps
bool param_list(T_token token, T_queue *queue, FILE *file) {
    printf("<parm_list>\n");
    // <param>
    if (param(token, queue, file))
    {
        printf("<p-list>\n");

        // <p-list>
        token = getToken(queue, file);
        return p_list(token, queue, file);
    // eps
    } else {
        printf("eps\n");

        queue_add(queue, token);
        return true;
    }
    return false;
}

// <p-list> -> , <param> <p-list>
// <p-list> -> eps
bool p_list(T_token token, T_queue *queue, FILE *file){
    printf("<p_list>\n");
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
    printf("<param>\n");
    // p-name
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
                token = getToken(queue, file);

                if (token.type == TOKEN_KW_DOUBLE || token.type == TOKEN_KW_STRING || token.type == TOKEN_KW_INT || token.type == TOKEN_TYPE_FLOAT || token.type == TOKEN_TYPE_INT || token.type == TOKEN_TYPE_STRING)
                {
                    return true;
                }
            }
        }        
    }
    return false;
}
