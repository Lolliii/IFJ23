/*
IFJ Projekt 2023

CODE GENERATOR

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "code_gen.h"

// Spocita kolikaty je poslany frame
int count_frames(Tlist *list){
    int count = 0;

    if(list->act != NULL){
        while(list->act->rPtr != NULL){
            count++;
            list->act = list->act->rPtr;
        }
    }

    return (count > 0) ? 1 : 0;
}

// Vypise var
void printVar(int frame, int var){
    if(!frame){
        printf(" GF@_%d" , var);
    } else {
        if(frame == 1){
            printf(" LF@_%d", var);
        } else {
            printf(" TF@_%d", var);
        } 
    }
}

// Vypise symb
// ! nil ??
void printSymb(bool id, int symbFrame, int symbVar, char value[], int type){
    if(!id){   
        if(type == TOKEN_KW_STRING){
            printf(" string@%s", value);
        } else {
            if(type == TOKEN_KW_INT){
                int vall = atoi(value);
                printf(" int@%d", vall);
            } else {
                if(type == TOKEN_KW_DOUBLE){
                    float val = atof(value);
                    printf(" float@%a", val);
                } else {
                    printf(" nil@nil");
                }
            }
        }
    } else {           // ID
        printVar(symbFrame, symbVar);
    }
}


// Vytvoří nový dočasný rámec a zahodí případný obsah původního dočasného rámce.
void createFrame(void){
    printf("\nCREATEFRAME");
}

// Presune TF na zasobnik ramcu, bude k dispozici pres LF
// Pripadne prekryje ostatni
// Po teto instrukci je TF nedefinovan, je potreba pouzit CREATEFRAME
void pushFrame(void){
    printf("\nPUSHFRAME");
}

// Presune vrcholovy LF ze zasobniku do TF
void popFrame(void){
    printf("\nPOPFRAME");
}

// Vrati se odkud se volalo
void cReturn(void){
    printf("\nRETURN");
}

// Smaze datovy zasobnik
void clears(void){
    printf("\nCLEARS");
}

// Zasobnikove ADD
void adds(void){
    printf("\nADDS");
}

// Zasobnikove SUB
void subs(void){
    printf("\nSUBS");
}

// Zasobnikove MUL
void muls(void){
    printf("\nMULS");
}

// Zasobnikove DIV
void divs(void){
    printf("\nDIVS");
}

// Zasobnikove IDIV
void idivs(void){
    printf("\nIDIVS");
}

// Zasobnikove LT (<)
void lts(void){
    printf("\nLTS");
}

// Zasobnikove GT (>)
void gts(void){
    printf("\nGTS");
}

// Zasobnikove EQ (==)
void eqs(void){
    printf("\nEQS");
}

// Zasobnikove AND
void ands(void){
    printf("\nANDS");
}

// Zasobnikove OR
void ors(void){
    printf("\nORS");
}

// Zasobnikove NOT 
void nots(void){
    printf("\nNOTS");
}

// Zasobnikove int -> float
void int2floats(void){
    printf("\nINT2FLOATS");
}

// Zasobnikove float -> int
void float2ints(void){
    printf("\nFLOAT2INTS");
}

// Zasobnikove prevedeni int na hodnotu ASCII
void int2chars(void){
    printf("\nINT2CHARS");
}

// Zasobnikove STRI2INT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
// Do <var> vlozi hodnotu ASCII znaku ze <symb1> na pozici <symb2>
void stri2ints(void){
    printf("\nSTRI2INTS");
}

// Vytvori navesti
void ifWhileLabel(int label){
    printf("\nLABEL %d", label);
}

// Vytvori navesti
void label(char label[]){
    printf("\nLABEL %s", label);
}

// Provede skok na navesti (funkce)
void callLabel(char label[]){
    printf("\nCALL %s", label);
}

// Provede nepodmineny skok na navesti 
void jump(char label[]){
    printf("\nJUMP %s", label);
}

// Provede nepodmineny skok na navesti 
void ifWhileJump(int label){
    printf("\nJUMP %d", label);
}

// Ukonci vykonavani programu s navratovym kodem returnCode 
// Rozsah <0, 49> (nekontrolovano)
void cExit(int returnCode){
    printf("\nEXIT %d", returnCode);
}

// Zasobnikovy skok na navesti, pokud rovno
void jumpIfEqS(int label){
    printf("\nJUMPIFEQS %d", label);
}

// Zasobnikovy skok na navesti, pokud nerovno
void jumpIfNEqS(int label){
    printf("\nJUMPIFNEQS %d", label);
}

// Vypise stav interpretu v danou chvili na stderr
void cBreak(void){
    printf("\nBREAK");
}

// Definuje promennou v urcenem ramci
// Promenna bude zatim neinicializovana
void defvar(int frame, int var){
    printf("\nDEFVAR");
    printVar(frame, var);
}

// Zasobnikove POP
// Pokud neni zasobnik prazdny, ulozi vrchol do var
void pops(int frame, int var){
    printf("\nPOPS");
    printVar(frame, var);
}

// Zasobnikove POP pro podminky
// Pokud neni zasobnik prazdny, ulozi vrchol do var
void popsCondition(void){
    printf("\nPOPS GF@_condition");
}


// Zasobnikove PUSH
// Ulozi <symb> na zasobnik
void pushs(bool id, int symbVar, int symbframe, char symb[], T_token_type type){
    printf("\nPUSHS");
    printSymb(id, symbframe, symbVar, symb, type);
}

// Zasobnikove PUSH pro vyhodnoceni podminek
void pushsCondition(char condition[]){
    printf("\nPUSHS bool@%s", condition);
}

// MOVE
void move(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nMOVE");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Prevede int z <symb> na float, a ulozi do <var>
void int2float(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nINT2FLOAT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Prevede float z <symb> na int, a ulozi do <var>
void float2int(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nFLOAT2INT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// int <symb> je podle ascii ulozena na char, a ulozena do <var>
void int2char(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nINT2CHAR");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Do <var> ulozi ASCII hodnotu znaku ze <symb1> na pozici <symb2>
void stri2int(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nSTRI2INT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Vypise hodnotu <symb> na stdout
void write(bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nWRITE");
    printSymb(id, symbframe, symbVar, symb, type);
}

// Delku <symb> ulozi do <var>
void cStrlen(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nSTRLEN");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Typ <symb> ulozi do <var>
void type(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nTYPE");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Vypise hodnotu <symb> na stderr
void dPrint(bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nDPRINT");
    printSymb(id, symbframe, symbVar, symb, type);
}

// ADD (stejny typ)
void add(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nADD");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// SUB (stejny typ)
void sub(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nSUB");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// MUL (stejny typ)
void mul(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nMUL");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// DIV (stejny typ)
void cDiv(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nDIV");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// SUB (stejny typ)
void idiv(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nIDIV");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Less Than (stejny typ)
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void lt(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nLT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Greater Than (stejny typ)
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void gt(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nGT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// EQual (stejny typ)
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void eq(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nEQ");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// AND - logicke na BOOL <symb>
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void and(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nAND");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// OR- logicke na BOOL <symb>
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void or(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nOR");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// NOT - logicke na BOOL <symb>
// ! jen jeden operand
// ! vraci to do bool <var>
// ! podivej se radsi do zadani
void not(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type){
    printf("\nNOT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
}

// Konkatenace retezcu
void concat(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nCONCAT");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Do <var> ulozi retezec z jednoho znaku z retezce <symb1> na pozici <symb2>
void cGetChar(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nGETCHAR");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Zmeni znak v <var> na znak z <symb1> z pozice <symb2> (prvni, pokud obsahuje vice znaku)
void cSetChar(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nSETCHAR");
    printVar(frame, var);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Pokud jsou <symbs> rovny, skoci se na label
void jumpIfEq(char label[], bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nJUMPIFEQ");
    printf(" %s", label);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Pokud nejsou <symbs> rovny, skoci se na label
void jumpIfNEq(char label[], bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2){
    printf("\nJUMPIFNEQ");
    printf(" %s", label);
    printSymb(id, symbframe, symbVar, symb, type);
    printSymb(id2, symbframe2, symbVar2, symb2, type2);
}

// Nacte ze vstupu hodnotu do <var>, podle typu <type>
// Kompatibilni s vestavenymi funkcemi readString, readInt, readDouble
// <type> -> {int, float, string, bool}
void cRead(int frame, int var, char type[]){
    printf("\nREAD");
    printVar(frame, var);
    printf(" %s", type);
}

// Vypise zahlavi .IFJcode23
void codeGenInit(void){
    printf(".IFJcode23");

    // ??
    createFrame();
    pushFrame();
}

// Ukonceni generovani
void codeGenFinish(void){
    clears();
    popFrame();
}

