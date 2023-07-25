#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

int main (int argc, char *argv[] ){
    printf("Quanta memoria occupano i vari tipi di dato:\n");
    printf("Char: %lu byte\nInt: %lu\nLong: %lu\nFloat: %lu\n", sizeof(char), sizeof(int), sizeof(long), sizeof(float));
    printf("\n*Char: %lu byte\n*Int: %lu\n*Long: %lu\n*Float: %lu\n", sizeof(char*), sizeof(int*), sizeof(long*), sizeof(float*));
    
    //variabili
    int i = 9;
    char c = 'a';
    
    //puntatori
    int *pi;
    char *pc;
    
    //inserisco gli indirizzi nei puntatori
    pi = &i;
    pc = &c;

    //stampo gli indirizzi
    printf("\nIdirizzo di memoria di i: %p\nIdirizzo di memoria di c: %p\n",pi, pc );
    
    printf("\nIl valore di i è %d=%d\n", i, *pi);
    printf("Il valore di c è %c=%c\n", c, *pc);

    //modifico il valore di i attraverso la variabile
    i = 12;
    printf("\nIl valore modificato di i è %d=%d\n", i, *pi);
    //modifico il contenuto della cella puntata attraverso il puntatore
    *pi = 19;
    printf("\nIl valore modificato di i è %d=%d\n", i, *pi);

    return 0;
}