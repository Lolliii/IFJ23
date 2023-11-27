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

// ! Kod se bude generovat na stdout
// ! Budu vyuzivat printf

// <var>   -> Neterminal -> promenna
// <symb>  -> Konstanta/promenna
// <label> -> Navesti
// Identifikator promenne se sklada z FRAME@jmeno
//      -> jmeno zacina pismenem, nebo specialnim znakem (_, -, $, &, %, *, !, ?) -> (plati i pro label)

// Vypise zahlavi .IFJcode23
int codeGenInit();

// NEMELY BY BRAT PARAMETRY
void createFrame();
void pushFrame();
void popFrame();
void cReturn();
void clears();
void adds();
void subs();
void divs();
void idivs();
void lts();
void gts();
void eqs();
void ands();
void ors();
void nots();
void int2floats();
void float2ints();
void int2chars();
void stri2ints();
void label(char *label);
void call(char *label);
void jump(char *label);
void cExit(int returnCode);
void jumpIfEqS(char *label);
void jumpIfNEqS(char *label);


// void cBreak();







#endif