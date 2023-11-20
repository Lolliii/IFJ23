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
#include <stdbool.h> // nebo pouzit #define OK 1 notok 0?????
#include "scanner.h"


// budem potrebovat keywordy z getNextToken v scanneru, typ float int a string nevim ci je potreba

//terminaly z LL gramatiky
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
bool f_type(T_token token, T_queue *queue, FILE *file);
bool param_list(T_token token, T_queue *queue, FILE *file);
bool p_list(T_token token, T_queue *queue, FILE *file);
bool param(T_token token, T_queue *queue, FILE *file);

#endif
