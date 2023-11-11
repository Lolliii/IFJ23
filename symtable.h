#ifndef SYMSTACK_H
#define SYMSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct bStrom{
    void *data;          //pozor ked sa bude robit s touto premenou treba ju pretipovat podla toho co do nej budeme ukladat
    int key;
    int height;
    bStrom *lPtr;
    bStrom *rPtr;
}bStrom;

int max(int n1, int n2);    //vracia vecsie cislo
int bHeightBS(bStrom *root);    //vracia vysku stromu
int bVyvazenost(bStrom *root);  //vracia 0 ak je vyvazeni 1 ak je lavi podstrom o jedno tazsi alebo -1 ak je pravi tazsi
bStrom *bCreate(int key, void *data);//vytvori novy prvok
bool bChecknext(bStrom *root);//pozrie sa ci je root NULL, pouzitie pre pozretie sa dopredu
bStrom *bRightRotate(bStrom *root);//otoci do prava
bStrom *bLeftRotate(bStrom *root);//otoci do lava
bStrom *bInsert(bStrom *root, int key, void *data);//vlozi prvok
bStrom *bMinL(bStrom *root);//vrati najavejsi prvok stormu
bStrom *bMinR(bStrom *root);//vrati najpravejsi prvok stromu
bStrom *bDeleteOne(bStrom *root, int key);//odstrani jeden
bStrom *bDestroyR(bStrom *root);//zrusi cely strom
void bPreOrder(bStrom *root);//vypisuje
void bInOrder(bStrom *root);
void bPostOrder(bStrom *root);

#endif
