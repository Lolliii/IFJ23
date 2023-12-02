/*
IFJ Projekt 2023

PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "parser.h"

bool is_def = false;

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

// Funkce pro porovnání NILable typů s normálními
// Protože expression parser vrací pouze typy bez ?
bool IsTokenTypeCheck(T_token_type type, T_token_type result)
{
    if((result == TOKEN_KW_INT && type == TOKEN_TYPE_INT) ||
    (result == TOKEN_KW_DOUBLE && type == TOKEN_TYPE_FLOAT) ||
    (result == TOKEN_KW_STRING && type == TOKEN_TYPE_STRING))
        return true;
    else
        return false;
}

// <prog> -> <st-list> 
bool prog(T_token token, T_queue *queue, FILE *file) {
    // Incializace symtable
    Tlist *sym_list;
    sym_list = init_list();
    bStrom *sym_table = NULL;
    add_to_Lil(sym_list, sym_table);
    set_act_first_Lil(sym_list);


    if(!st_list(token, queue, file, sym_list)){
        error_caller(SYN_ERROR);
        exit(SYN_ERROR);
    }

    // TODO: Zrusit symtable

    return true;
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list) {
    // EOF
    if (token.type == TOKEN_EOF) {
        return true;
    } else {
        T_func funkce;
        funkce.name = NULL;
        // <stat>
        if (stat(token, queue, file, funkce, sym_list))
        {
            // <st-list>
            token = getToken(queue, file);
            if (st_list(token, queue, file, sym_list)) return true;
        }
    }
    return false;
}

// <body> -> <stat> <body>
// <body> -> eps
bool body(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list) {
    // <stat>
    if (stat(token, queue, file, *funkce, sym_list))
    {
        // <body>
        token = getToken(queue, file);
        if (body(token, queue, file, funkce, sym_list))
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
bool stat(T_token token, T_queue *queue, FILE *file, T_func funkce, Tlist *sym_list) {
    // <stat> -> while <exp-stat> { <body> }
    if (token.type == TOKEN_KW_WHILE)
    {
        // <exp-stat>
        token = getToken(queue, file);
        if (exp_stat(token, queue, file, sym_list))
        {
            // {
            token = getToken(queue, file);
            if (token.type == TOKEN_L_CURL)
            {
                // Vytvoření nového rámce
                bStrom *sym_table_frame = NULL;
                add_to_Lil(sym_list, sym_table_frame);
                set_act_first_Lil(sym_list);

                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file, &funkce, sym_list))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
                        // Zrušení rámce
                        destroy_Lilfirst(sym_list);
                        set_act_first_Lil(sym_list);
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
        if (exp_stat(token, queue, file, sym_list))
        {
            // {
            token = getToken(queue, file);
            if (token.type == TOKEN_L_CURL)
            {
                // Vytvoření nového rámce
                bStrom *sym_table_frame = NULL;
                add_to_Lil(sym_list, sym_table_frame);
                set_act_first_Lil(sym_list);

                // <body>
                token = getToken(queue, file);
                if (body(token, queue, file, &funkce, sym_list))
                {
                    // }
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_CURL)
                    {
                        // Zrušení rámce
                        destroy_Lilfirst(sym_list);
                        set_act_first_Lil(sym_list);

                        // else
                        token = getToken(queue, file);
                        if (token.type == TOKEN_KW_ELSE)
                        {
                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
                                // Vytvoření nového rámce
                                bStrom *sym_table_frame = NULL;
                                add_to_Lil(sym_list, sym_table_frame);
                                set_act_first_Lil(sym_list);

                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file, &funkce, sym_list))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
                                        // Zrušení rámce
                                        destroy_Lilfirst(sym_list);
                                        set_act_first_Lil(sym_list);
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
            // Nasla se definice funkce, napln strukturu pro symtable
            // TODO: zkontrolovat jestli uz nebyla definovana
            funkce.param_count = 0;
            funkce.name = token.value;
            
            // Vytvoření nového rámce
            bStrom *sym_table_frame = NULL;
            add_to_Lil(sym_list, sym_table_frame);
            set_act_first_Lil(sym_list);

            // (
            token = getToken(queue, file);
            if (token.type == TOKEN_L_RND)
            {
                // <param-list>
                token = getToken(queue, file);
                if (param_list(token, queue, file, &funkce, sym_list))
                {
                    // )
                    token = getToken(queue, file);
                    if (token.type == TOKEN_R_RND)
                    {
                        // <f-type>
                        token = getToken(queue, file);
                        if (f_type(token, queue, file, &funkce, sym_list))
                        {
                            // {
                            token = getToken(queue, file);
                            if (token.type == TOKEN_L_CURL)
                            {
                                /* 
                                TODO: v tomto miste mame vsechny potrebne informace o funkci a muzeme ji ulozit do tabulky
                                TODO zatim jen vypisuji informace o funkci, pak smazat*/
                                /*printf("%s\n", funkce.name);
                                printf("%d\n", funkce.param_count);
                                printf("%d\n", funkce.returnType);
                                for(int i = 0; i < funkce.param_count; i++)
                                {
                                    if(funkce.params[i].pName == NULL)
                                        printf("NULL\t");
                                    else
                                        printf("%s\t", funkce.params[i].pName);
                                    printf("%s\t", funkce.params[i].paramId);
                                    printf("%i\n", funkce.params[i].pType);

                                }*/
                                
                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file, &funkce, sym_list))
                                {
                                    // }
                                    token = getToken(queue, file);
                                    if (token.type == TOKEN_R_CURL)
                                    {
                                        // Zrušení rámce
                                        destroy_Lilfirst(sym_list);
                                        set_act_first_Lil(sym_list);
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
        // Deklarace ID, napln sturkturu pro symtable
        T_id id;
        id.modifiable = 0;                  // typ LET
        id.type = 999;                      // Proměnná ještě nemá přiřazený typ -> hodnota 999
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            id.name = token.value;
            // Kontrola jestli proměnná již není v daném framu
            bStrom *tmp = bsearch_one(sym_list->first->data, id.name);
            if(tmp != NULL)
            {
                // Pokud již existuje -> chyba            
                error_caller(UNDEF_REDEF_ERROR);
                exit(UNDEF_REDEF_ERROR);
            }

            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file, id, sym_list)) return true;
        }
        return false;
        
    // <stat> -> var id <id-type>
    } else if (token.type == TOKEN_KW_VAR) {
        // Deklarace ID, napln sturkturu pro symtable
        // id
        T_id id;
        id.modifiable = 1;              // typ VAR
        id.type = 999;                  // Proměnná ještě nemá přiřazený typ -> hodnota 999
        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // <id-type>
            id.name = token.value;
            // Kontrola jestli proměnná již není v daném framu
            bStrom *tmp = bsearch_one(sym_list->first->data, id.name);
            if(tmp != NULL)
            {
                // Pokud již existuje -> chyba     
                error_caller(UNDEF_REDEF_ERROR);
                exit(UNDEF_REDEF_ERROR);
            }

            // <id-type>
            token = getToken(queue, file);
            if(id_type(token, queue, file, id, sym_list)) return true;
        }
        return false;
        
    // <stat> -> id <id-stat>
    } else if (token.type == TOKEN_ID) {
        // <id-stat>
        // Mám pouze ID, může to být přiřazení, nebo volání funkce
        T_id id;
        id.name = token.value;          // Pro pozdější vyhledání si uložím název ID

        token = getToken(queue, file);
        if(id_stat(token, queue, file, id, sym_list)) return true;
        
        return false;

    // <stat> -> return <ret-stat>
    } else if (token.type == TOKEN_KW_RETURN) {
        // <ret_stat>
        token = getToken(queue, file);
        if(ret_stat(token, queue, file, funkce, sym_list)) return true;

        return false;
    }
    return false;
}

// <ret-stat> -> <exp>
// <ret-stat> -> eps
bool ret_stat(T_token token, T_queue *queue, FILE *file, T_func funkce, Tlist *sym_list) {
    // <exp>
    bool is_void = true;  // Predpokladame ze funkce je void
    if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_R_CURL || token.type == TOKEN_L_RND)
    {
        // Za return je nějaký expression -> funkce není void
        is_void = false;
        queue_add(queue, token);
    }

    if(!is_void)
    {
        // Funkce není void, podívám se co za typ má vracet a co vrátil expression parser
        T_token_type result = expr_parser(file, queue, sym_list);

        // Sedí návratový typ? 
        if(result != funkce.returnType && !IsTokenTypeCheck(funkce.returnType, result))
        {
            error_caller(PARAM_ERROR);
            exit(PARAM_ERROR);
        }
    }
    else
    {
        // Je funkce opravdu void?
        if(funkce.returnType != TOKEN_VOID)
        {
            error_caller(PARAM_ERROR);
            exit(PARAM_ERROR);
        }
    }
    return true;
}

//<id-type> -> : type <assign>
//<id-type> -> = <call>
bool id_type(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list){
    // V deklaraci je i typ promenne
    // :    
    if(token.type == TOKEN_COLON)
    {
        // type
        T_token tmp = getToken(queue, file);
        if (IsType(tmp))
        {
            id.type = tmp.type;             // Už znám i typ proměnné
            // <assign>
            T_token next_token = getToken(queue, file);
            return assign(next_token, queue, file, id, sym_list);
        }
    // =
    }else if(token.type == TOKEN_ASSIGN){
        // <call>
        token = getToken(queue, file);
        return call(token, queue, file, id, sym_list);
    }else{
        return false;
    }
    return false;
}

// <assign> -> = <call>
// <assign> -> eps
bool assign(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list){
    // =
    if(token.type == TOKEN_ASSIGN){
        // <call>
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file, id, sym_list);
    // eps
    }else{  
        // Pouze deklarace, vloz do symtable, není inicializovaná
        id.initialized = false;
        sym_list->first->data = bInsert(sym_list->first->data, id.name, (void*)&id, 3);
        queue_add(queue, token);
        return true;
    }
    return false;
}

//<id-stat> -> = <call>
//<id-stat> –> ( <term-list> )
bool id_stat(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list){
    // =
    if(token.type == TOKEN_ASSIGN){
        // <call>
        // Je to přiřazení do proměnné
        id.modifiable = -1;         // Nevím jestli to je LET nebo VAR, protože nejsem v deklaraci -> hodnota -1
        // Kontola jestli proměnná existuje (v jakémkoliv frame)
        ListElement *frame = bSearch_all(sym_list, id.name);
        if(frame == NULL)
        {
            error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
            exit(UNDEF_UNINIT_VARIABLE_ERROR);
        }
        // Proměnná existuje -> zjistim její typ ze symtable pro pozdější kontrolu
        bStrom *item = bsearch_one(frame->data, id.name);
        T_id *item_id1 = (T_id*)item->data;
        id.type = item_id1->type;

        // Proměnná je definovaná typu LET -> nemodifikovatelná
        if(!item_id1->modifiable)
        {
            error_caller(OTHER_ERROR);
            exit(OTHER_ERROR);
        }

        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file, id, sym_list);
    // (
    }else if(token.type == TOKEN_L_RND){    // Vola se funkce bez prirazeni
        // <term-list>
        // TODO: zkontroluj navratovy typ funkce (mel by to byt nejspis void)
        T_token next_token = getToken(queue, file);
        if(term_list(next_token, queue, file, sym_list)){
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
bool call(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list){
    // id
    if(token.type == TOKEN_ID){
        // kouknu dopredu na dalsi token
        T_token tmp = getToken(queue, file);
        // id ( --> volani funkce
        // TODO: Prirazeni -> zkontroluj navratovy typ funkce a typ promenne
        if (tmp.type == TOKEN_L_RND)
        {
            token = tmp;
            // <term-list>
            token = getToken(queue, file);
            if(term_list(token, queue, file, sym_list))
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
            T_token_type result = expr_parser(file, queue, sym_list);
            id.initialized = true;
            // Nyní mám celou definici proměnné nebo přiřazení do proměnné

            // Pokud není typ definován (zůstala hodnota 999), definuje se z návratové hodnoty expressionu
            // Jinak se zkontroluje jestli souhlasí s typem expressionu
            if(id.type == 999)
                id.type = result;
            else
            {
                if((id.type != result && !IsTokenTypeCheck(id.type, result)))
                {
                    error_caller(TYPE_COMP_ERROR);
                    exit(TYPE_COMP_ERROR);
                }
            }
            
            bStrom *item = bsearch_one(sym_list->first->data, id.name);

            // Pokud proměnná ještě není definována v symtable, tak ji vlož
            // Pokud je id.modifiable == -1, to znamená, že proměnná již existuje v jiném rámci -> nevkládej
            if(item == NULL && id.modifiable != -1)
                sym_list->first->data = bInsert(sym_list->first->data, id.name, (void*)&id, 3);
            
            // Proměnná již je definovaná
            if(item != NULL)
            {
                T_id *item_id1 = (T_id*)item->data;
                // Kontrola jestli typ proměnné a vráceného výrazu je stejný
                if(item_id1->type == result || IsTokenTypeCheck(item_id1->type, result))
                {
                    // Proměnná již je definovaná, ale nebyla inicializovaná (nyní už je)
                    if(!item_id1->initialized)
                    {
                        ListElement *frame = bSearch_all(sym_list, item_id1->name);
                        bStrom *cur_root = bsearch_one(frame->data, item_id1->name);
                        bUpdate_root(cur_root, true);
                    }
                }
                else    // Typy nejsou shodné
                {
                    error_caller(TYPE_COMP_ERROR);
                    exit(TYPE_COMP_ERROR);
                }
            }
            return true;
        }
        return false;
    // <exp>
    }else{
        if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
        }
        T_token_type result = expr_parser(file, queue, sym_list);
        id.initialized = true;
        // Nyní mám celou definici proměnné nebo přiřazení do proměnné
        
        // Pokud není typ definován (zůstala hodnota 999), definuje se z návratové hodnoty expressionu
        // Jinak se zkontroluje jestli souhlasí s typem expressionu
        if(id.type == 999)
            id.type = result;
        else
        {
            if((id.type != result && !IsTokenTypeCheck(id.type, result)))
            {
                error_caller(TYPE_COMP_ERROR);
                exit(TYPE_COMP_ERROR);
            }
        }
        
        bStrom *item = bsearch_one(sym_list->first->data, id.name);

        // Pokud proměnná ještě není definována v symtable, tak ji vlož
        // Pokud je id.modifiable == -1, to znamená, že proměnná již existuje v jiném rámci -> nevkládej
        if(item == NULL && id.modifiable != -1)
            sym_list->first->data = bInsert(sym_list->first->data, id.name, (void*)&id, 3);

        // Proměnná již je definovaná
        if(item != NULL)
        {
            T_id *item_id1 = (T_id*)item->data;
            // Kontrola jestli typ proměnné a vráceného výrazu je stejný
            if(item_id1->type == result || IsTokenTypeCheck(item_id1->type, result))
            {
                // Proměnná již je definovaná, ale nebyla inicializovaná (nyní už je)
                if(!item_id1->initialized )
                {
                    ListElement *frame = bSearch_all(sym_list, item_id1->name);
                    bStrom *cur_root = bsearch_one(frame->data, item_id1->name);
                    bUpdate_root(cur_root, true);
                }
            }
            else    // Typy nejsou shodné
            {
                error_caller(TYPE_COMP_ERROR);
                exit(TYPE_COMP_ERROR);
            }
        }
        return true;
    }
    return false;
}

// <exp-stat> -> <exp>
// <exp-stat> -> let id
bool exp_stat(T_token token, T_queue *queue, FILE *file, Tlist *sym_list){
    // let
    if(token.type == TOKEN_KW_LET){
        // id
        T_token tmp = getToken(queue, file);
        if(tmp.type == TOKEN_ID){
            // Kontrola jestli je LET ID definované a jestli se jedná o nemodifikovatelnou proměnnou
            ListElement *frame = bSearch_all(sym_list, tmp.value);
            if(frame == NULL)
            {
                error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
                exit(UNDEF_UNINIT_VARIABLE_ERROR);
            }
            bStrom *item = bsearch_one(frame->data, tmp.value);
            T_id *item_id1 = (T_id*)item->data;

            // Proměnná je definovaná typu VAR, my potřebujeme LET
            if(item_id1->modifiable)
            {
                error_caller(OTHER_ERROR);
                exit(OTHER_ERROR);
            }
            return true;
        }
        return false;
    // <exp>
    }else{
        if (IsTerm(token) || token.type == TOKEN_ID || token.type == TOKEN_ID_EM || token.type == TOKEN_L_RND)
        {
            queue_add(queue, token);
        }
        // ! neco udelat s navratovou hodnotou
        T_token_type result = expr_parser(file, queue, sym_list);
        if(result != TOKEN_BOOL)
        {
            error_caller(OTHER_ERROR);
            exit(OTHER_ERROR);
        }
        return true;
    }
    return false;
}

// <term-list> -> <term> <t-list>
// <term-list> -> eps
bool term_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list) {
    // ! paramtery funkce
    // TODO: Zkontoluj volane parametry a ty v definici + navratovy typ a typ promenne
    // <term>
    if (term(token, queue, file, sym_list))
    {
        // <t-list>
        token = getToken(queue, file);
        return t_list(token, queue, file, sym_list);
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
bool t_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list) {
    // TODO: Pokracovani kontroly z term_list
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <term>
        token = getToken(queue, file);
        if (term(token, queue, file, sym_list)) {
            // <t-list>
            token = getToken(queue, file);
            return t_list(token, queue, file, sym_list);
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
bool term(T_token token, T_queue *queue, FILE *file, Tlist *sym_list){
    // id
    if (token.type == TOKEN_ID) {
        // <term-name>
        token = getToken(queue, file);
        return term_name(token, queue, file, sym_list);

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
bool term_name(T_token token, T_queue *queue, FILE *file, Tlist *sym_list) {
    // TODO: kontola parametru
    // :
    (void) sym_list;
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
bool f_type(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list) {
    // ->
    (void) sym_list;
    if (token.type == TOKEN_FUNCTION_TYPE)
    {
        // type - Double, Int, String, ?
        token = getToken(queue, file);
        if (IsType(token))
        {
            funkce->returnType = token.type;
            return true;
        }
        return false;
    // eps
    } else {
        funkce->returnType = TOKEN_VOID;
        queue_add(queue, token);
        return true;
    }   
    return false;
}

// <param-list> -> <param> <p-list>
// <param-list> -> eps
bool param_list(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list) {
    // TODO: doplnit do struktury T_func
    // <param>
    if (param(token, queue, file, funkce, sym_list))
    {
        // <p-list>
        token = getToken(queue, file);
        return p_list(token, queue, file, funkce, sym_list);
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <p-list> -> , <param> <p-list>
// <p-list> -> eps
bool p_list(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list){
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <param>
        token = getToken(queue, file);
        if (param(token, queue, file, funkce, sym_list))
        {
            // <p-list>
            token = getToken(queue, file);
            return p_list(token, queue, file, funkce, sym_list);
        }
    // eps
    } else {
        queue_add(queue, token);
        return true;
    }
    return false;
}

// <param> -> p-name id : type
bool param(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list){
    // p-name, _
    if (token.type == TOKEN_ID || token.type == TOKEN_UNDERSCORE)
    {
        /* Jméno parametru funkce, se kterým se volá,
           pokud je '_' -> nastavíme ho ve struktuře na NULL*/
        if(token.type == TOKEN_ID)
            funkce->params[funkce->param_count].pName = token.value;
        else
            funkce->params[funkce->param_count].pName = NULL;

        token = getToken(queue, file);
        if (token.type == TOKEN_ID)
        {
            // Identifikátor parametru v těle funkce
            funkce->params[funkce->param_count].paramId = token.value;
            // :
            token = getToken(queue, file);
            if (token.type == TOKEN_COLON)
            {
                // type - Double, Int, String, ?
                token = getToken(queue, file);
                if (IsType(token))
                {
                    // Typ parametru funkce
                    funkce->params[funkce->param_count].pType = token.type;

                    /* Protože parametry funkce, jsou zároveň i lokálními proměnnými funkce
                       uložíme je do tabulky symbolů, lokální frame již je vytvořený */
                    T_id id = {.name = funkce->params[funkce->param_count].paramId,
                               .initialized = true,
                               .type = funkce->params[funkce->param_count].pType,
                               .modifiable = 1};
                    sym_list->first->data = bInsert(sym_list->first->data, id.name, (void*)&id, 3);
                    
                    // Zvýšení počítadla parametrů funkce
                    funkce->param_count += 1;
                    return true;
                }
            }
        }        
    }
    return false;
}
