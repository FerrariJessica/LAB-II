#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

// stampa un messaggio d'errore su stderr e termina il programma
void termina(char *messaggio)
{
  // se errno!=0 oltre al mio messaggio stampa il messaggio
  // associato alla variabile globale errno 
  // utilizzando la funzione di libreria perror()
  if(errno!=0) perror(messaggio);
  // ltrimenti stampa solo il mio messaggio
  else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}

int *elabora(int a[], int n, int k, int *nuovon){
    assert (a!=NULL);
    assert(n>0);
    int *b, m;
    if(k == 0){
        m = n;
        b = malloc(m*sizeof(int));
        if (b == NULL) termina("errore nella malloc");
        for(int i=0; i<n; i++){
            b[i] = 0;
        }
    }else if(k>0){
        int preso = 0;
        m = n*k; 
        b = malloc(m*sizeof(int));
        if (b == NULL) termina("errore nella malloc");
        for(int i=0; i<m; i++){
            if(i%k == 0){
                b[i] = a[preso];
                preso += 1 ;
            }else b[i] = b[i-1];
        }
    }else if(k<0){
        int preso = n-1;
        k = -k; 
        m = n*k;
        b = malloc(m*sizeof(int));
        if (b == NULL) termina("errore nella malloc");
        for(int i=0; i<m; i++){
            if(i%k == 0){
                b[i] = a[preso];
                preso -= 1 ;
            }else b[i] = b[i-1];
        }
    }
    *nuovon = m;
    return b;
}




int main (int argc, char *argv[] ){
    if(argc <4){
        printf("il programma deve essere eseguito: main nomeFileIn k1 nomeFileOut1 ...");
        return 1;
    }

    //apro il file di lettura
    char *nomeFileIn = argv[1];
    FILE *fileIn = fopen(nomeFileIn, "r");
    if (fileIn == NULL) termina("Errore nell'apertura del file");


    //leggo gli elementi del file e li metto in a[];
    int size = 10; 
    int messi = 0;
    int *a = malloc(size*sizeof(int));
    if (a==NULL) termina ("errore nella malloc di a");

    while (true)
    {
        int n;
        int e = fscanf(fileIn,"%d",&n);
        
        //file terminato
        if(e == EOF) break;
        //contenuto non valido
        if(e != 1) termina ("Contenuto non valido nel file");

        if(messi == size){
            size = size*2;
            a = realloc(a, size*sizeof(int));
            if(a==NULL) termina("errore nella realloc");
        }

        a[messi] = n;
        messi +=1;
    }
    //ho terminato di leggere dal file
    size = messi;
    a = realloc(a, size*sizeof(int));
    if(a == NULL) termina("errore nella realloc");

    

    //chiudo il file di lettura
    fclose(fileIn);


    //chiamo elabora per ogni coppia e inserisco nel rispettivo file
    for(int i=2; i<argc; i+=2 ){
        char *nomeFileOut = argv[i+1];
        FILE *fileOut = fopen(nomeFileOut, "wt");
        if (fileOut == NULL ) termina("errore apertura file output");
        int nuovon;
        int k = atoi(argv[i]);
        int* b = elabora(a, size, k, &nuovon);
        //scrivo nel file gli elementi di nuovo
        for(int i=0; i<nuovon; i++){
            int e = fprintf(fileOut, "%d\n", b[i]);
            if (e<0) termina("errore nella scrittura del file");
        }

        //chiudo il file
        fclose(fileOut);
        free(b);

    }

    //prova stdout, stderr, stdin
    printf("solo per provare\n");
    fprintf(stdout, "questo si scrive diretto\n");
    fprintf(stderr,"questo va negli errori\n");


    free(a);
    return 0;

}