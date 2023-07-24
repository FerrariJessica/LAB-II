/*
* Leggere un intero N da tastiera
* Creare un array di N int 
* Leggere da tastiera N interi e inserirli nell'array
* Calcolare la somma degli elementi dell'array
* Stampare la somma
*/

#define _GNU_SOURCE  // avverte che si usano le estensioni GNU
#include <stdio.h>   // printf, scanf 
#include <stdlib.h>  // malloc, free, exit, conversioni stringa/numero
#include <stdbool.h> // gestisce il tipo bool (quindi per le variabili true, false)
#include <string.h>  // gestisce le stringhe, con funzioni di confronto/copia/etc
#include <assert.h>  // permette di usare la funzione assert

int main(int argc, char *argv[]){
    //definosco la variabile n
    int n;

    //leggo da tastiera il valore di n
    printf("Inserisci il numero di elementi: ");
    int e = scanf("%d", &n);
    if(e != 1){
        puts("Valore non trovato\n");
        exit(1); //termina il programma con codice 1
    }
    if(n <= 0){
        puts("Il numero di elementi deve essere positivo\n");
        exit(1); 
    }

    //creo e riempio l'array
    int a[n];
    for(int i=0; i<n; i++){
        printf("inserisci l'elemento di posto %d: ", i);
        e = scanf("%d", &a[i]);
        if(e!=1){
            puts("Valore non trovato\n")
            exit(1);
        }
    }

     //calcolo la somma 
        int somma = 0;
        for(int i=0; i<n; i++){
            somma += a[i];
        }
    //stampo la somma
    printf("La somma totale è :%d\n", somma);

    //termino il programma
    return 0;    
}