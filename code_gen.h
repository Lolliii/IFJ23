/*
IFJ Projekt 2023

CODE GENERATOR

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef CODE_GEN_H
#define CODE_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "symtable.h"


// <var>   -> Neterminal -> promenna
// predava se jako v parametrech jako:
//           int frame (0 = GF, 1 = LF, 2 = TF)
//           int var - cislo promenne (ve funkci bude vzdy na zacatek pridano _)

// <symb>  -> Konstanta/promenna
// predava se jako v parametrech jako:
//           bool id (0 = konstanta, 1 = id)
//           int symbFrame (same jako u <var>)
//           int symbVar (same jako u <var>)
//           char symb[] -> hodnoty se ukladaji do token.value jako string
//           int type

// <label> -> Navesti
// Identifikator promenne se sklada z FRAME@jmeno
//      -> jmeno zacina pismenem, nebo specialnim znakem (_, -, $, &, %, *, !, ?) -> (plati i pro label)

int count_frames(Tlist *list);

// Vypise zahlavi .IFJcode23
void codeGenInit(void);
void codeGenFinish(void);

void createFrame(void);
void pushFrame(void);
void popFrame(void);
void cReturn(void);
void clears(void);
void adds(void);
void subs(void);
void muls(void);
void divs(void);
void idivs(void);
void lts(void);
void gts(void);
void eqs(void);
void ands(void);
void ors(void);
void nots(void);
void int2floats(void);
void float2ints(void);
void int2chars(void);
void stri2ints(void);
void ifWhileLabel(int label);
void label(char label[]);
void callLabel(char label[]);
void jump(char label[]);
void ifWhileJump(int label);
void cExit(int returnCode);
void jumpIfEqS(int label);
void jumpIfNEqS(int label);
void cBreak(void);

void defvar(int frame, int var);
void pops(int frame, int var);
void popsCondition(void);
void move(int frame, int var, bool id, int symbVar, int symbframe ,char symb[], int type);
void pushs(bool id, int symbVar, int symbframe, char symb[], T_token_type type);
void pushsCondition(char condition[]);
void int2float(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);
void float2int(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);
void int2char(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);
void stri2int(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void write(bool id, int symbVar, int symbframe, char symb[], int type);
void cStrlen(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);
void type(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);
void dPrint(bool id, int symbVar, int symbframe, char symb[], int type);

void add(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void sub(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void mul(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void cDiv(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void idiv(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);

void lt(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void gt(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void eq(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);

void and(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void or(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void not(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type);

void concat(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void cGetChar(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void cSetChar(int frame, int var, bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);

void jumpIfEq(char label[], bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void jumpIfNEq(char label[], bool id, int symbVar, int symbframe, char symb[], int type, bool id2, int symbVar2, int symbframe2, char symb2[], int type2);
void cRead(int frame, int var, char type[]);

#endif
