/*
IFJ Projekt 2023

SYMBOL TABLE

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef SYMSTACK_H
#define SYMSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include <stdbool.h>
#include <string.h>

#define type_in_tree 4;

typedef struct bStrom{
    void *data;          //pozor ked sa bude robit s touto premenou treba ju pretipovat podla toho co do nej budeme ukladat
    char *key;
    int height;
    int data_typ;       //tu treba definovat co tam vkladame 1 pre T_pram 2 pre T_func 3 pre T_id
    struct bStrom *lPtr;
    struct bStrom *rPtr;
}bStrom;

typedef struct param{
    char *pName;                // Jmeno parametru
    char *paramId;              // ID parametru
    T_token_type pType;
    // int generated_id;
}T_param;

typedef struct func {
    char *name;                 // Nazev fuknce
    T_token_type returnType;
    struct param params[20];   // Parametry (je potreba se na ne odkazovat takhle foo.params[0]->pName)
    int param_count;           // Počet parametrů funkce
}T_func;

typedef struct id{
    char *name;                 // Nazev promenne
    bool  initialized;          // Je inicializovana?
    T_token_type type;
    int  modifiable;            // 0 -> let, 1 -> var, -1 -> ani jedno
    int generated_id;           // Nazev, pod kterym se dane ID vygenerovalo 
}T_id;


int max(int n1, int n2);    //vracia vecsie cislo
int bHeightBS(bStrom *root);    //vracia vysku stromu
int bVyvazenost(bStrom *root);  //vracia 0 ak je vyvazeni 1 ak je lavi podstrom o jedno tazsi alebo -1 ak je pravi tazsi
bStrom *bCreate(char *key, void *data, int data_type);//vytvori novy prvok
bool bChecknext(bStrom *root);//pozrie sa ci je root NULL, pouzitie pre pozretie sa dopredu
bStrom *bRightRotate(bStrom *root);//otoci do prava
bStrom *bLeftRotate(bStrom *root);//otoci do lava
bStrom *bInsert(bStrom *root, char *key, void *data, int data_type);//vlozi prvok
bStrom *bMinL(bStrom *root);//vrati najavejsi prvok stormu
bStrom *bMinR(bStrom *root);//vrati najpravejsi prvok stromu
bStrom *bDeleteOne(bStrom *root, char *key);//odstrani jeden
void bDestroyR(bStrom *root);//zrusi cely strom        // pridana * za void
void bPreOrder(bStrom *root);//vypisuje
void bInOrder(bStrom *root);
void bPostOrder(bStrom *root);
bStrom *bsearch_one(bStrom *root, char *search);
bStrom *bUpdate_root(bStrom *root, bool init);

//tu je list
typedef struct ListE{
    bStrom *data;
    struct ListE *rPtr;
}ListElement;

typedef struct tlist{
    ListElement *act;
    ListElement *first;
}Tlist;


Tlist *init_list(void); //inicalizacie treba pouzit ako prve
//za add_to_Lil treba pouzit hned set_act_first_Lil
void add_to_Lil(Tlist *t, bStrom *data);//vlozi na prvi prvok treba pouzit na zaciatku
void set_act_first_Lil(Tlist *t);//nastavi act na prvi prvok
bool isActive_Lil(Tlist *t);//bool true ak je act
void InsertAfter_Lil(Tlist *t, bStrom *data);//vlozi prvok za aktivni
void set_value_Lil(Tlist *t, bStrom *data);
void DeleteAfter_Lil(Tlist *t);
void next_Lil(Tlist *t);
void destroy_LilLast(Tlist *t); //destroy the last element
void destroy_Lilall(Tlist *t); //will destroy the whole list
ListElement *bSearch_all(Tlist *t, char *search);//perjde vsetko
void destroy_Lilfirst(Tlist *t);

#endif
