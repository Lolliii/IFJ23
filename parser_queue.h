/*
IFJ Projekt 2023

Hlavičkový soubor pro lineární ADT fronta

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef PARSER_QUEUE_H
#define PARSER_QUEUE_H

#include "scanner.h"

/* Struktura pro prvek ve frontě*/
typedef struct telem_queue
{
    T_token token;                  // Data prvku ve frontě
    struct telem_queue *next;       // Ukazatel na další prvek
}T_queue_elem;

/* Struktura pro samotnou frontu s ukazateli na začátek a konec*/
typedef struct tqueue
{
    T_queue_elem *beg;
    T_queue_elem *end;
}T_queue;

/* Funkce pro inicializaci fronty, dynamicky naalokuje prostor a nastaví ukazatele na NULL
   Bez parametrů
   Vrací ukazatel na vytvořenou frontu*/
T_queue *queue_init(void);

/* Funkce pro přidání prvku(tokenu) na konec fronty
   Parametry jsou: ukazatel na frontu, kam chceme prvek přidat a samotný prvek
   Bez návratové hodnoty*/
void queue_add(T_queue *queue, T_token token);

/* Funkce pro odebrání prvku z vrcholu fronty
   Parametrem je ukazatel na frontu
   Bez návratové hodnoty*/
void queue_remove(T_queue *queue);

/* Funkce pro získání prvku z vrcholu fronty
   Parametrem je ukazatel na frontu
   Vrací token z vrcholu fronty*/
T_token queue_front(T_queue *queue);

/* Funkce pro získání dalšího tokenu z fronty nebo souboru
   Parametry jsou ukazatel na frontu a ukazatel na soubor
   Vrací token*/
T_token getToken(T_queue *queue, FILE *file);

#endif
