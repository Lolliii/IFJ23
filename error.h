/*
IFJ Projekt 2023

ERROR

@author Jakub Valeš
@author Milan Takáč
@author Jakub Dyrčík
@author Thu Tra Phamová
*/

#ifndef ERROR_H
#define ERROR_H

// • 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
// • 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, chybějící hlavička, atp.).
// • 3 - sémantická chyba v programu – nedefinovaná funkce, pokus o redefinice funkce.
// • 4 - sémantickáchyba v programu – špatný počet/typ parametrů u volání funkce či typ návratové hodnoty z funkce.
// • 5 - sémantická chyba v programu – použití nedefinované nebo neinicializovane proměnné.
// • 6 - sémantická chyba v programu – chybějící/přebývající výraz v příkazu návratu z funkce.
// • 7 - sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
// • 8 - sémantická chyba odvození typu – typ proměnné nebo parametru není uveden a nelze odvodit od použitého výrazu.
// • 9 - ostatní sémantické chyby.
// • 99 - interní chyba překladače tj. neovlivněná vstupním programem (např. chyba alokace paměti atd.).

#define NO_ERROR 0
#define LEX_ERROR 1
#define SYN_ERROR 2
#define UNDEF_FUNCTION_ERROR 3
#define PARAM_ERROR 4
#define UNDEF_UNINIT_VARIABLE_ERROR 5
#define EXPRESSION_ERROR 6
#define TYPE_COMP_ERROR 7
#define MISSING_TYPE 8
#define OTHER_ERROR 9
#define INTER_ERROR 99

#endif
