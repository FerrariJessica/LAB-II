//rifaccio l'esercizio dei numeri primi senza guardare 
//copio solo l'intestazione

#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe

bool primo(int k)
{
  for(int i=2; i<k; i++)
    if(k%i == 0) return false; // ho scoperto che il numero non è primo
  return true;
}

int main(int argc, char *argv[]){
    int n;

    //chiedo di inserire un numero n da tastiera
    printf ("Inserire un numero : ");
    int e = scanf("%d", &n);

    //guardo se l'ho letto bene 
    if(e != 1) printf("valore non trovato");


    //creo l'array in memoria dinamica
    int *array;
    int size = 10;
    int messi = 0;

    array = (int*) malloc(size*sizeof(int));

    if(array==NULL){
        printf("Malloc fallita");
        exit(1);
    } 
   
    //controllo tutti i numeri da 1 a n 
    for(int i=2; i<=n; i++){
       
        //se è primo controllo che l'array non sia pieno e se è pieno lo raddoppio
        if(primo(i)){
            if(messi == size){
                size = 2*size;
                array = (int*) realloc(array, size*sizeof(int));
                if (array == NULL) {
                    printf("realloc fallita");
                    exit(1);
                }
            }
            //aggiungo l'elemento
            array[messi] = i;
            messi += 1;
            
        }
    }


    //riduco lo spazio dell'array in memoria
    array = (int*) realloc(array, messi*sizeof(int));
    if (array == NULL) {
        printf("realloc fallita");
        exit(1);
    }
            
    //stampo l'array dei primi 
    printf("i numeri primi minori di %d sono: ", n);
    for(int i=0; i<messi; i++){
        printf("%8d", array[i]);
    }
    printf("\n");

    //libero la memoria
    free(array);
    return 0;

}
