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

    // Strom pro deklarace funkcí
    Tlist *fun_list;
    fun_list = init_list();
    bStrom *fun_tree = NULL;
    add_to_Lil(fun_list, fun_tree);
    set_act_first_Lil(fun_list);

    // List pro definice funkcí, které ještě nebyli deklarovány
    Tlist *fun_call_list;
    fun_call_list = init_list();


    if(!st_list(token, queue, file, sym_list, fun_list, fun_call_list)){
        error_caller(SYN_ERROR);
        exit(SYN_ERROR);
    }

    // TODO: Zrusit symtable

    return true;
}

// <st-list> -> <stat> <st-list>
// <st-list> -> EOF
bool st_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list) {
    // EOF
    if (token.type == TOKEN_EOF) {
        return true;
    } else {
        T_func funkce;
        funkce.name = NULL;
        // <stat>
        if (stat(token, queue, file, funkce, sym_list, fun_list, fun_call_list))
        {
            // <st-list>
            token = getToken(queue, file);
            if (st_list(token, queue, file, sym_list, fun_list, fun_call_list)) return true;
        }
    }
    return false;
}

// <body> -> <stat> <body>
// <body> -> eps
bool body(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list) {
    // <stat>
    if (stat(token, queue, file, *funkce, sym_list, fun_list, fun_call_list))
    {
        // <body>
        token = getToken(queue, file);
        if (body(token, queue, file, funkce, sym_list, fun_list, fun_call_list))
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
bool stat(T_token token, T_queue *queue, FILE *file, T_func funkce, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list) {
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
                if (body(token, queue, file, &funkce, sym_list, fun_list, fun_call_list))
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
                if (body(token, queue, file, &funkce, sym_list, fun_list, fun_call_list))
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
                                if (body(token, queue, file, &funkce, sym_list, fun_list, fun_call_list))
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
            funkce.param_count = 0;
            funkce.name = token.value;
            // Zkontoluj jestli již není definovaná
            bStrom *fun_tmp = bsearch_one(fun_list->first->data, funkce.name);
            if(fun_tmp != NULL)
            {
                error_caller(OTHER_ERROR);
                exit(OTHER_ERROR);
            }
            
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
                                // Naplnění stromu s definicemi funkcí
                                fun_list->first->data = bInsert(fun_list->first->data, funkce.name, (void*)&funkce, 2);
                                // TODO: zkontrolovat list s voláním, funkcí, které ještě nejsou definované
                                // TODO: bStrom *found_fun = bsearch_one(fun_tree, funkce.name);

                                // TODO: T_func *fce = (T_func*)found_fun->data;
                                
                                // <body>
                                token = getToken(queue, file);
                                if (body(token, queue, file, &funkce, sym_list, fun_list, fun_call_list))
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
            if(id_type(token, queue, file, id, sym_list, fun_list)) return true;
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
            if(id_type(token, queue, file, id, sym_list, fun_list)) return true;
        }
        return false;
        
    // <stat> -> id <id-stat>
    } else if (token.type == TOKEN_ID) {
        // <id-stat>
        // Mám pouze ID, může to být přiřazení, nebo volání funkce
        T_id id;
        id.name = token.value;          // Pro pozdější vyhledání si uložím název ID

        token = getToken(queue, file);
        if(id_stat(token, queue, file, id, sym_list, fun_list, fun_call_list)) return true;
        
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
bool id_type(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list){
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
            return assign(next_token, queue, file, id, sym_list, fun_list);
        }
    // =
    }else if(token.type == TOKEN_ASSIGN){
        // <call>
        token = getToken(queue, file);
        return call(token, queue, file, id, sym_list, fun_list);
    }else{
        return false;
    }
    return false;
}

// <assign> -> = <call>
// <assign> -> eps
bool assign(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list){
    // =
    if(token.type == TOKEN_ASSIGN){
        // <call>
        T_token next_token = getToken(queue, file);
        return call(next_token, queue, file, id, sym_list, fun_list);
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
bool id_stat(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list, Tlist *fun_called_list){
    // =
    // PŘIŘAZENÍ DO PROMĚNNÉ
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
        return call(next_token, queue, file, id, sym_list, fun_list);
    
    // VOLÁNÍ FUNKCE
    // (
    }else if(token.type == TOKEN_L_RND){    // Vola se funkce bez prirazeni
        // <term-list>
        // Jedná se o volání funkce, plním strukturu pro funce
        T_func fun_called;
        fun_called.param_count = 0;
        fun_called.name = id.name;          // Jméno funkce bylo v předchozím tokenu a je uloženo v id.name

        T_token next_token = getToken(queue, file);
        if(term_list(next_token, queue, file, sym_list, &fun_called)){
            // )
            token = getToken(queue, file);
            if(token.type == TOKEN_R_RND){

                (void) fun_called_list;
                bStrom *fun_tmp = bsearch_one(fun_list->first->data, fun_called.name);
                
                // FUNKCE UŽ JE DEFINOVANA -> kontrola
                if(fun_tmp != NULL)
                {
                    T_func *def_fun = (T_func*)fun_tmp->data;
                    if(def_fun->param_count == fun_called.param_count)
                    {
                        for(int i = 0; i < def_fun->param_count; i++)
                        {
                            if(def_fun->params[i].pType == fun_called.params[i].pType)
                            {
                                if(def_fun->params[i].pName == NULL && fun_called.params[i].pName == NULL)
                                    continue;
                                else if(def_fun->params[i].pName != NULL && fun_called.params[i].pName != NULL)
                                {
                                    if(strcmp(def_fun->params[i].pName, fun_called.params[i].pName) == 0)
                                    {
                                        continue;
                                    }
                                }
                            }
                            error_caller(PARAM_ERROR);
                            exit(PARAM_ERROR);
                        }
                    }
                    else
                    {
                        error_caller(PARAM_ERROR);
                        exit(PARAM_ERROR);
                    }
                }
                // JEŠTĚ NENÍ DEFINOVANÁ
                else
                {
                    printf("NENI DEF\n");   
                }
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
bool call(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list){
    // id
    if(token.type == TOKEN_ID){
        // kouknu dopredu na dalsi token
        T_token tmp = getToken(queue, file);
        // id ( --> volani funkce
        // TODO: Prirazeni -> zkontroluj navratovy typ funkce a typ promenne
        if (tmp.type == TOKEN_L_RND)
        {
            T_func fun_called;
            fun_called.name = token.value;
            fun_called.param_count = 0;

            token = tmp;
            // <term-list>
            token = getToken(queue, file);
            if(term_list(token, queue, file, sym_list, &fun_called))
            {
                // )
                token = getToken(queue, file);
                if(token.type == TOKEN_R_RND){
                    bStrom *fun_tmp = bsearch_one(fun_list->first->data, fun_called.name);
                    
                    // FUNKCE UŽ JE DEFINOVANA -> kontrola
                    if(fun_tmp != NULL)
                    {
                        T_func *def_fun = (T_func*)fun_tmp->data;
                        if(def_fun->param_count == fun_called.param_count)
                        {
                            for(int i = 0; i < def_fun->param_count; i++)
                            {
                                if(def_fun->params[i].pType == fun_called.params[i].pType)
                                {
                                    if(def_fun->params[i].pName == NULL && fun_called.params[i].pName == NULL)
                                        continue;
                                    else if(def_fun->params[i].pName != NULL && fun_called.params[i].pName != NULL)
                                    {
                                        if(strcmp(def_fun->params[i].pName, fun_called.params[i].pName) == 0)
                                        {
                                            continue;
                                        }
                                    }
                                }
                                error_caller(PARAM_ERROR);
                                exit(PARAM_ERROR);
                            }
                        }
                        else
                        {
                            error_caller(PARAM_ERROR);
                            exit(PARAM_ERROR);
                        }

                        // VLOŽENÍ DO SYMTABLE
                        id.initialized = true;
                        // Nyní mám celou definici proměnné nebo přiřazení do proměnné

                        // Pokud není typ definován (zůstala hodnota 999), definuje se z návratové hodnoty expressionu
                        // Jinak se zkontroluje jestli souhlasí s typem expressionu
                        if(id.type == 999)
                            if(def_fun->returnType == TOKEN_VOID)
                            {
                                // TODO pridat do error caller
                                error_caller(MISSING_TYPE_ERROR);
                                fprintf(stderr, "ERROR -> 8: Missing type\n");
                                exit(MISSING_TYPE_ERROR);
                            }
                            else
                                id.type = def_fun->returnType;
                        else
                        {
                            if((id.type != def_fun->returnType && !IsTokenTypeCheck(id.type, def_fun->returnType)))
                            {
                                if(!IsTokenTypeCheck(def_fun->returnType, id.type))
                                {
                                    error_caller(TYPE_COMP_ERROR);
                                    exit(TYPE_COMP_ERROR);
                                }
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
                            if(item_id1->type == def_fun->returnType || IsTokenTypeCheck(item_id1->type, def_fun->returnType))
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
                    }
                    else
                    {
                        printf("NENI DEF\n");
                        return true;
                    }



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
bool term_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called) {
    // ! paramtery funkce
    // <term>
    if (term(token, queue, file, sym_list, fun_called))
    {
        // <t-list>
        token = getToken(queue, file);
        return t_list(token, queue, file, sym_list, fun_called);
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
bool t_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called) {
    // TODO: Pokracovani kontroly z term_list
    // ,
    if (token.type == TOKEN_COMMA)
    {
        // <term>
        token = getToken(queue, file);
        if (term(token, queue, file, sym_list, fun_called)) {
            // <t-list>
            token = getToken(queue, file);
            return t_list(token, queue, file, sym_list, fun_called);
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
bool term(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called){
    // id
    if (token.type == TOKEN_ID) {
        // <term-name>
        // Funkce má nějaký pName
        fun_called->params[fun_called->param_count].pName = token.value;
        //token = getToken(queue, file);
        return term_name(token, queue, file, sym_list, fun_called);

    // lit
    } else if (IsTerm(token) || token.type == TOKEN_KW_NIL) {
        // Je to literál, nastavím hodnotu token.type jako typ parametru funkce
        fun_called->params[fun_called->param_count].pType = token.type;
        switch (fun_called->params[fun_called->param_count].pType)
        {
        case TOKEN_INT:
        case TOKEN_INT_EXP:
        case TOKEN_INT_EXP_PM:
            fun_called->params[fun_called->param_count].pType = TOKEN_KW_INT;
            break;
        case TOKEN_DOUBLE:
        case TOKEN_DOUBLE_EXP:
        case TOKEN_DOUBLE_EXP_PM:
            fun_called->params[fun_called->param_count].pType = TOKEN_KW_DOUBLE;
            break;
        case TOKEN_STRING:
        case TOKEN_ML_STRING:
            fun_called->params[fun_called->param_count].pType = TOKEN_KW_STRING;
            break;
        default:
            break;
        }
        fun_called->param_count++;
        return true;
    } else {
        return false;
    }
    return false;
}

// <term-name> -> : val
// <term-name> -> eps 
bool term_name(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called) {
    // TODO: kontola parametru
    // :
    T_token token_2 = getToken(queue, file);
    if (token_2.type == TOKEN_COLON)
    {
        // Mám
        // Nový token
        token = getToken(queue, file);
        if(IsTerm(token) || token.type == TOKEN_ID) {
            if(token.type == TOKEN_ID)
            {
                // Je to identifikátor, najdu ho v tabulce symbolů a zjistím jeho typ
                    ListElement *frame = bSearch_all(sym_list, token.value);
                    if(frame != NULL){
                        // hledani v strome
                        bStrom *found = bsearch_one(frame->data, token.value);
                        T_id *id_data = (T_id *)(found->data);
                        // Nastavím typ proměnné do typu ID ve volání funkce
                        fun_called->params[fun_called->param_count].pType = id_data->type;
                    }
                    else
                    {
                        error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
                        exit(UNDEF_UNINIT_VARIABLE_ERROR);
                    }
                
            }
            else
            {
                // Je to literál, nastavím hodnotu token.type jako typ parametru funkce
                fun_called->params[fun_called->param_count].pType = token.type;
                switch (fun_called->params[fun_called->param_count].pType)
                {
                    case TOKEN_INT:
                    case TOKEN_INT_EXP:
                    case TOKEN_INT_EXP_PM:
                        fun_called->params[fun_called->param_count].pType = TOKEN_KW_INT;
                        break;
                    case TOKEN_DOUBLE:
                    case TOKEN_DOUBLE_EXP:
                    case TOKEN_DOUBLE_EXP_PM:
                        fun_called->params[fun_called->param_count].pType = TOKEN_KW_DOUBLE;
                        break;
                    case TOKEN_STRING:
                    case TOKEN_ML_STRING:
                        fun_called->params[fun_called->param_count].pType = TOKEN_KW_STRING;
                        break;
                    default:
                        break;
                }
            }
            fun_called->param_count++;
            return true;
        }
    // eps
    } else {
        // V token je ještě token z předchozí funkce, ID
        fun_called->params[fun_called->param_count].pName = NULL;
        ListElement *frame = bSearch_all(sym_list, token.value);
        if(frame != NULL){
            // hledani v strome
            bStrom *found = bsearch_one(frame->data, token.value);
            T_id *id_data = (T_id *)(found->data);
            // Nastavím typ proměnné do typu ID ve volání funkce
            fun_called->params[fun_called->param_count].pType = id_data->type;
            fun_called->param_count++;
        }
        else
        {
            error_caller(UNDEF_UNINIT_VARIABLE_ERROR);
            exit(UNDEF_UNINIT_VARIABLE_ERROR);
        }
        queue_add(queue, token_2);
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
