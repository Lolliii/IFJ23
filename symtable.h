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
#include <stdbool.h>



typedef struct bStrom{
    void *data;          //pozor ked sa bude robit s touto premenou treba ju pretipovat podla toho co do nej budeme ukladat
    char key;
    int height;
    struct bStrom *lPtr;
    struct bStrom *rPtr;
}bStrom;

typedef struct param{
    char *pName;                // Jmeno parametru
    char *paramId;              // ID parametru
    int   pType;                // Typ -> Int(0), String(1), Double(2)??
} param;

typedef struct func {
    char *name;                 // Nazev fuknce
    int   returnType;           // Typ -> Int(0), String(1), Double(2), void(3)??
    struct param *params[20];   // Parametry (je potreba se na ne odkazovat takhle foo.params[0]->pName)
} func;

typedef struct id{
    char *name;                 // Nazev promenne
    bool  initialized;          // Je inicializovana?
    int   type;                 // Typ -> Int(0), String(1), Double(2)
    bool  modifiable;           // Lze menit?
} id;

// Priklad 
// param parameter;
// parameter.pName = "jebej";
// parameter.paramId = "klokan";
// parameter.pType = 1;

// printf("%s\n", parameter.pName);
// printf("%s\n", parameter.paramId);
// printf("%d\n", parameter.pType);

// func function;
// function.name = "zavolej";
// function.returnType = 1;
// function.params[0] = &parameter;

// printf("%s\n", function.name);
// printf("%d\n", function.returnType);
// printf("%s\n", function.params[0]->pName);


int max(int n1, int n2);    //vracia vecsie cislo
int bHeightBS(bStrom *root);    //vracia vysku stromu
int bVyvazenost(bStrom *root);  //vracia 0 ak je vyvazeni 1 ak je lavi podstrom o jedno tazsi alebo -1 ak je pravi tazsi
bStrom *bCreate(char key, void *data);//vytvori novy prvok
bool bChecknext(bStrom *root);//pozrie sa ci je root NULL, pouzitie pre pozretie sa dopredu
bStrom *bRightRotate(bStrom *root);//otoci do prava
bStrom *bLeftRotate(bStrom *root);//otoci do lava
bStrom *bInsert(bStrom *root, char key, void *data);//vlozi prvok
bStrom *bMinL(bStrom *root);//vrati najavejsi prvok stormu
bStrom *bMinR(bStrom *root);//vrati najpravejsi prvok stromu
bStrom *bDeleteOne(bStrom *root, char key);//odstrani jeden
void *bDestroyR(bStrom *root);//zrusi cely strom        // pridana * za void
void bPreOrder(bStrom *root);//vypisuje
void bInOrder(bStrom *root);
void bPostOrder(bStrom *root);
bStrom *bsearch_one(bStrom *root, char search);


//tu je list
typedef struct ListE{
    bStrom *data;
    struct ListE *rPtr;
}ListElement;

typedef struct tlist{
    ListElement *act;
    ListElement *first;
}Tlist;


Tlist *init_list(); //inicalizacie treba pouzit ako prve
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
ListElement *bSearch_all(Tlist *t, char search);//perjde vsetko

#endif
