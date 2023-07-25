//rifaccio il programma della lezione 1 ma usando la memoria dinamica
//  legge un intero N da tastiera
//  crea un array *dinamico* di N int
//  legge N interi mettendoli nell'array
//  calcola la somma degli elementi dell'array
//  stampa la somma 

#define _GNU_SOURCE  // avverte che si usano le estensioni GNU
#include <stdio.h>   // printf, scanf 
#include <stdlib.h>  // malloc, free, exit, conversioni stringa/numero
#include <stdbool.h> // gestisce il tipo bool (quindi per le variabili true, false)
#include <string.h>  // gestisce le stringhe, con funzioni di confronto/copia/etc
#include <assert.h>  // permette di usare la funzione assert

int main(int argc, char *argv[]){
    int n;

    printf("Inserire il numero di elementi con cui fare la somma : ");
    int e = scanf("%d", &n);
    if(e != 1) {
        printf("Valore non trovato");
        exit(1);
    }
    if(n<=0) {
        puts("Il numero di elementi deve essere positivo");
        exit(2);
    }

    int *a;
    a = (int*) malloc(n*sizeof(int));
    if(a==NULL) {
        puts("Malloc fallita");
        exit(3);
    } 

    //riempio l'array
    for(int i=0; i<n; i++){
        printf("Inserisci l'elemento di posto %d: ",i);                    
        e = scanf("%d", &a[i]);
        if(e!=1) {
            puts("Valore non trovato");
            exit(1);
        }
    }

    //calcolo e stampo la somma
    int somma = 0;
    for(int i=0; i<n; i++){
        somma += a[i];
    }

    printf("La somma dei numeri inseriti è %d\n", somma);

    //dealloco la memoria
    free(a);
    
    return 0;
}