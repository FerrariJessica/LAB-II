#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>

/* TESTO ESERCIZIO:
    Scrivere un programma che legge una sequenza di interi sulla linea di comando 
    e stampa il più grande valore che appaia almeno 2 volte. 
    Il programma deve stampare un messaggio di errore se nessun valore appare almeno due volte. 
   
   Esempi:
    max2v 3 -5 7 2 7 2 2 -5 100 2 1000 -5
    -> 7  [solo  7, 2 e -5 appaiono piu' di una volta]
    max2v 3 -5 7 -2 17 -5 100 -2 1000 -5
    -> -2 [solo -2 e -5 appaiono piu' di una volta]
    max2v 100 1000 1 2 3 -4
    -> Errore, nessun valore appare almeno 2 volte

*/



// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}

/*int *doppi(int a[], int n, int *m, int *sb){
    //dato un array di interi a[] restituisce un array di interi b[] 
    //contenente gli elementi almeno doppi di a[]
    //calcolare il massimo degli elementi di b[] 
    int *b = NULL;
    int messi = 0;
    int max = -100;
    b = malloc(n*sizeof(int));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (a[i] == a[j]) {
                bool presente = false;
                for (int y = 0; y < messi; y++) {
                    if (b[y] == a[i]) {
                        presente = true;
                        break;
                    }
                }
                if (!presente) {
                    
                    b[messi] = a[i];
                    if (a[i] > max) {
                        max = a[i];
                    }
                    messi++;
                }
            }
        }
    }

    b = realloc(b, messi * sizeof(int));
    if (b == NULL) {
        termina("Realloc fallita");
    }

    *sb = messi;
    *m = max;
    return b;

}*/





/*int main(int argc, char *argv[])
{
   
    
     
    int max;
    int sizeb;
    int *b = 0;
    b = doppi(a, size, &max, &sizeb);

    if(sizeb == 0){
        fprintf(stderr, "Errore, non ci sono carratteri che si ripetono");
    } else{
        fprintf(stdout, "Il massimo dei valori che si ripetono è : %d\n Si ripetono i valori: ", max);
        for(int i=0; i<sizeb; i++){
            printf("%d ", b[i]);
        }
    }

    free(a);
    free(b);


    return 0;
}*/
bool presente(int *a, int size, int n){
    for(int i=0; i<size; i++){
        if(a[i]==n) return true;
    }
    return false;
}


int *doppi(int *a, int n, int *max, int *nb){
    int messi = 0;
    int m = 0;

    int *b = malloc(n*sizeof(int));
    
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(a[i]==a[j]){
                if(!presente(b, messi, a[i])){
                    b[messi] = a[i];
                    messi ++;
                    if(a[i]>m) m = a[i];
                }
                
            }
        }
    }


    if(n!=messi){
        b = realloc(b, messi*sizeof(int));
    }

    *max = m;
    *nb = messi;
    return b;

}


int main(int argc, char const *argv[])
{
    if(argc < 3){
        fprintf(stderr, "Uso: %s i1, i2, ...\n", argv[0]);
        exit(0);
    }


    int max = 0;
    int sizeb = 0 ;
    int size = argc-1;
    int *a = malloc(size*sizeof(int));
    if (a==NULL) termina("Allocazione di a fallita");

    //riempio l'array
    for(int i=0; i<size; i++){
        a[i] = atoi(argv[i+1]);
    }
    
    //stampa a
    printf("Array a: ");
    for(int i=0; i<size; i++){
        printf("%d ", a[i]);
    }

    //creo b
    int *b = doppi(a, size, &max, &sizeb);

    if(sizeb == 0) {
        printf("\nNon ci sono doppi\n");
    }
    else{
        //stampa b
        printf("\nGli elementi ripetuti sono: ");
        for(int i=0; i<sizeb; i++){
            printf("%d ", b[i]);
        }

        //stampo il massimo
        printf("\nEd il massimo è %d\n", max);
    }

    free(a);
    free(b);

    return 0;
}
