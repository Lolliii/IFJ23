/*
IFJ Projekt 2023

CODE GENERATOR

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#include "code_gen.h"

// TODO: bude se id zrovna posilat i s framem, nebo na to dat zvlast parametr?

// Vypise zahlavi .IFJcode23
int codeGenInit(){
    printf(".IFJcode23");
}

// NEMELY BY BRAT PARAMETRY

// Vytvoří nový dočasný rámec a zahodí případný obsah původního dočasného rámce.
void createFrame(){
    printf("\nCREATEFRAME");
}

// Presune TF na zasobnik ramcu, bude k dispozici pres LF
// Pripadne prekryje ostatni
// Po teto instrukci je TF nedefinovan, je potreba pouzit CREATEFRAME
void pushFrame(){
    printf("\nPUSHFRAME");
}

// Presune vrcholovy LF ze zasobniku do TF
void popFrame(){
    printf("\nPOPFRAME");
}

// Vrati se odkud se volalo
void cReturn(){
    printf("\nRETURN");
}

// Smaze datovy zasobnik
void clears(){
    printf("\nCLEARS");
}

// Zasobnikove ADD
void adds(){
    printf("\nADDS");
}

// Zasobnikove SUB
void subs(){
    printf("\nSUBS");
}

// Zasobnikove DIV
void divs(){
    printf("\nDIVS");
}

// Zasobnikove IDIV
void idivs(){
    printf("\nIDIVS");
}

// Zasobnikove LT (<)
void lts(){
    printf("\nLTS");
}

// Zasobnikove GT (>)
void gts(){
    printf("\nGTS");
}

// Zasobnikove EQ (==)
void eqs(){
    printf("\nEQS");
}


// Zasobnikove AND
void ands(){
    printf("\nANDS");
}


// Zasobnikove OR
void ors(){
    printf("\nORS");
}


// Zasobnikove NOT 
void nots(){
    printf("\nNOTS");
}


// Zasobnikove int -> float
void int2floats(){
    printf("\nINT2FLOATS");
}

// Zasobnikove float -> int
void float2ints(){
    printf("\nFLOAT2INTS");
}

// Zasobnikove prevedeni int na hodnotu ASCII
void int2chars(){
    printf("\nINT2CHARS");
}

// Zasobnikove STRI2INT ⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
// Do <var> vlozi hodnotu ASCII znaku ze <symb1> na pozici <symb2>
void stri2ints(){
    printf("\nSTRI2INTS");
}

// Vytvori navesti
void label(char *label){
    printf("\nLABEL %s", label);
}

// Provede skok na navesti (funkce)
void call(char *label){
    printf("\nCALL %s", label);
}

// Provede nepodmineny skok na navesti 
void jump(char *label){
    printf("\nJUMP %s", label);
}

// Ukonci vykonavani programu s navratovym kodem returnCode 
// Rozsah <0, 49> (nekontrolovano)
void cExit(int returnCode){
    printf("\nEXIT %d", returnCode);
}

// Zasobnikovy skok na navesti, pokud rovno
void jumpIfEqS(char *label){
    printf("\nJUMPIFEQS %s", label);
}

// Zasobnikovy skok na navesti, pokud nerovno
void jumpIfNEqS(char *label){
    printf("\nJUMPIFNEQS %s", label);
}

// ! Tady to bude slozitejsi
// void cBreak(){
//     printf("\nBREAK");
// }