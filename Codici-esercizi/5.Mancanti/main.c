#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>

/*
TESTO:
    Scrivere un programma C, che legge dalla linea di comando il nome di un file di testo contenente interi a 32 bit, 
    copia gli interi in un array e stampa su stdout la somma degli interi compresi tra 1 e 1000 che non compaiono nell'array. 
    Ad esempio, se l'array contiene tutti gli interi tra 1 e 1000 tranne il 13, il 17, e il 49 il valore che deve essere stampato è 79 (la somma 13+17+49). 
    Si noti che tutti i valori dell'array non compresi tra 1 e 1000 non influenzano il risultato, e che il fatto che un valore compaia più volte nell'array non influenza il risultato: 
    a noi interessa la somma dei valori tra 1 e 1000 che non sono nell'array.

    Verificate con valgrind il corretto uso e deallocazione della della memoria
*/


// stampa un messaggio d'errore e termina il programma
void termina(const char *messaggio);



// scheletro del main della soluzione
int main(int argc, char *argv[])
{

  if(argc!=2) {
    fprintf(stderr, "Uso:\n\t   %s nome_file\n", argv[0]);
    exit(1);
  }

    
    int somma=0;
    
    FILE *f = fopen(argv[1], "r");
    if(f==NULL) termina("Errore nell'apertura del file");

    //creo un array che ha la funzione di bitmask
    int size = 10001;
    int *a = malloc(size*sizeof(int));
    for(int i=0; i<size; i++){
        a[i] = 0;
    }
    for(int i=0; i<size; i++){
        printf("a[%d] : %d ", i, a[i]);
    }


    //leggo il file :
    //se numero n è 1<=n<=1000 metto 1 nella posizione, altrimenti continuo a leggere il file
    int num;
    while(true){
        int e = fscanf(f, "%d", &num);
        if (e!=1) break;
        if(num <= 1000 && num >= 1){
            a[num] = 1;
        }
    }
    
    printf("\nArray dopo la lettura del file: \n");
    for(int i=0; i<size; i++){
        printf("a[%d] : %d ", i, a[i]);
    }
    
    fclose(f);

    for(int i=0; i<size; i++){
        if(a[i]==0){
            somma = somma + i;
        }
    }
    
    //fprintf(stderr,"Visualizzo il risultato (%d) su stdout:\n",somma);
	printf("\nLa somma degli elementi mancanti è: %d\n",somma);

    free(a);
   
  return 0;
}


void termina(const char *messaggio){
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}


