/*
IFJ Projekt 2023

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef PARSER_QUEUE_H
#define PARSER_QUEUE_H

#include "scanner.h"

// indexace tokenů v precedenční tabulce (+ je na indexu 0 atd...)


typedef struct telem_queue
{
    T_token token;
    struct telem_queue *next;
}T_queue_elem;

typedef struct tqueue
{
    T_queue_elem *beg;
    T_queue_elem *end;
}T_queue;


T_queue *queue_init(void);

void queue_add(T_queue *queue, T_token token);

void queue_remove(T_queue *queue);

T_token queue_front(T_queue *queue);

T_token getToken(T_queue *queue, FILE *file);

#endif
