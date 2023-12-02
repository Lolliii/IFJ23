/*
IFJ Projekt 2023

PARSER

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"
#include "parser_queue.h"
#include "expr_parser.h"
#include "symtable.h"

// Double, Int, String, ?
bool IsType(T_token token);
// numericke a retezcove literaly
bool IsTerm(T_token token);

// neterminaly z LL gramatiky
bool prog(T_token token, T_queue *queue, FILE *file);
bool st_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list);
bool body(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list);
bool stat(T_token token, T_queue *queue, FILE *file, T_func funkce, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list);
bool ret_stat(T_token token, T_queue *queue, FILE *file, T_func funkce, Tlist *sym_list);
bool id_type(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list);
bool assign(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list);
bool id_stat(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list, Tlist *fun_call_list);
bool call(T_token token, T_queue *queue, FILE *file, T_id id, Tlist *sym_list, Tlist *fun_list);
bool exp_stat(T_token token, T_queue *queue, FILE *file, Tlist *sym_list);
bool term_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called);
bool t_list(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called);
bool term(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called);
bool term_name(T_token token, T_queue *queue, FILE *file, Tlist *sym_list, T_func *fun_called);
bool f_type(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list);
bool param_list(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list);
bool p_list(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list);
bool param(T_token token, T_queue *queue, FILE *file, T_func *funkce, Tlist *sym_list);

#endif
