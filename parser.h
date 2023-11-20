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
bool prog(T_token token);
bool st_list(T_token token);
bool body(T_token token);
bool stat(T_token token);
bool ret_stat(T_token token);
bool id_type(T_token token);
bool assign(T_token token);
bool id_stat(T_token token);
bool call(T_token token);
bool exp_stat(T_token token);
bool term_list(T_token token);
bool t_list(T_token token);
bool term(T_token token);
bool f_type(T_token token);
bool param_list(T_token token);
bool p_list(T_token token);
bool param(T_token token);

#endif
