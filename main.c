/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
#include "error.h"
#include "expr_stack.h"
#include "expr_parser.h"
#include "parser_queue.h"
#include "parser.h"
#include "symtable.h"

#include <stdio.h>

    void printTree(bStrom *root) {
    if (root != NULL) {
        printTree(root->lPtr);
        printf("key: %s, data_typ: %d\n", root->key, root->data_typ);
        printTree(root->rPtr);
    }
}


int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    FILE* file = stdin;
    
    T_queue *queue = queue_init();
    
    //expr_parser(file, queue);

    T_token token = getNextToken(file);

// ______ parser
    prog(token, queue, file);

    free(queue);

    /*Tlist *sym_list;
    sym_list = init_list();
    bStrom *sym_table = NULL;
    add_to_Lil(sym_list, sym_table);
    set_act_first_Lil(sym_list);

    T_id id1;

    // Initialize id1
    id1.initialized = false;
    id1.type = -1;
    id1.name = "asdadfdf";

    // Insert id1
    sym_list->act->data = bInsert(sym_list->act->data, id1.name, (void*)&id1, 3);

    // Print the tree after the first insertion
    printf("After the first insertion:\n");
    printTree(sym_list->act->data);

    // Search for id1
    bStrom *tmp1 = bsearch_one(sym_list->act->data, id1.name);
    if (tmp1 != NULL) {
        T_id *tmp_id1 = (T_id*)tmp1->data;
        printf("id1: %s, type: %d, initialized: %d\n", tmp_id1->name, tmp_id1->type, tmp_id1->initialized);
    } else {
        printf("id1 not found\n");
    }

    // Modify id1 and insert again
    id1.initialized = true;
    id1.type = 42;
    id1.name = "another_id";

    sym_list->act->data = bInsert(sym_list->act->data, id1.name, (void*)&id1, 3);

    // Print the tree after the second insertion
    printf("After the second insertion:\n");
    printTree(sym_list->act->data);

    // Search using the modified name
    bStrom *tmp2 = bsearch_one(sym_list->act->data, "another_id");
    if (tmp2 != NULL) {
        T_id *tmp_id2 = (T_id*)tmp2->data;
        printf("id2: %s, type: %d, initialized: %d\n", tmp_id2->name, tmp_id2->type, tmp_id2->initialized);
    } else {
        printf("id2 not found\n");
    }
    bPreOrder(sym_list->act->data);
*/



    return 0;
}
