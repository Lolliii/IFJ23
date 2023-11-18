/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include <stdio.h>
#include <stdlib.h>

prec_symb get_prec_value(T_token token);

void reduce_rule(T_stack *stack, T_elem *cur_term);

void expr_parser(FILE* file);

#endif
