/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "error.h"
#include "scanner.h"
#include "expr_stack.h"
#include "expr_parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// #include <stdio.h>
// #include <stdlib.h>

prec_symb get_prec_value(T_token token, int *zavorka);

void reduce_rule(T_stack *stack, T_elem *cur_term);

void expr_parser(FILE* file, T_queue *queue);

#endif
