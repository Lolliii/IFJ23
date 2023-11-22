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

// neterminaly z LL gramatiky
bool prog(T_token token, T_queue *queue, FILE *file);
bool st_list(T_token token, T_queue *queue, FILE *file);
bool body(T_token token, T_queue *queue, FILE *file);
bool stat(T_token token, T_queue *queue, FILE *file);
bool ret_stat(T_token token, T_queue *queue, FILE *file);
bool id_type(T_token token, T_queue *queue, FILE *file);
bool assign(T_token token, T_queue *queue, FILE *file);
bool id_stat(T_token token, T_queue *queue, FILE *file);
bool call(T_token token, T_queue *queue, FILE *file);
bool exp_stat(T_token token, T_queue *queue, FILE *file);
bool term_list(T_token token, T_queue *queue, FILE *file);
bool t_list(T_token token, T_queue *queue, FILE *file);
bool term(T_token token, T_queue *queue, FILE *file);
bool term_name(T_token token, T_queue *queue, FILE *file);
bool f_type(T_token token, T_queue *queue, FILE *file);
bool param_list(T_token token, T_queue *queue, FILE *file);
bool p_list(T_token token, T_queue *queue, FILE *file);
bool param(T_token token, T_queue *queue, FILE *file);

#endif
