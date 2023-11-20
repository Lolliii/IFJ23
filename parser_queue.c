/*
IFJ Projekt 2023

queue

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "scanner.h"
#include "parser_queue.h"

#include <stdio.h>
#include <string.h>


T_queue *queue_init(void)
{
    T_queue *queue = malloc(sizeof(T_queue));
    if(queue == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    queue->beg = NULL;
    queue->end = NULL;
    return queue;
}


void queue_add(T_queue *queue, T_token token)
{
    T_queue_elem *new_elem = (T_queue_elem *) malloc(sizeof(T_queue_elem));
    if(new_elem == NULL)
    {
        fprintf(stderr, "MALLOC FAIL\n");
        exit(INTER_ERROR);
    }
    new_elem->token = token;
    new_elem->next = NULL;
    
    if(queue->beg == NULL)
        queue->beg = new_elem;
    else
        queue->end->next = new_elem;
    queue->end = new_elem;
}


void queue_remove(T_queue *queue)
{
    if(queue->beg != NULL){
        T_queue_elem *rm_elem = queue->beg;
        if(queue->beg == queue->end)
        {
            queue->end = NULL;
        }
        queue->beg = queue->beg->next;
        free(rm_elem);
    }
}


T_token queue_front(T_queue *queue)
{
    return queue->beg->token;
}

T_token getToken(T_queue *queue, FILE *file)
{
    T_token token;
    if(queue->beg == NULL)
    {
        token = getNextToken(file);
    }
    else
    {
        token = queue_front(queue);
        queue_remove(queue);
    }
    return token;
}
