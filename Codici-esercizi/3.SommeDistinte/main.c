#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>

/*
Testo esercizio
Scrivere una funzione C

int *somme_distinte(int a[], int n, int *size)

che dato un array di interi a[] di lunghezza n>0  alloca e restituisce un nuovo array b[] 
contenente tutti gli interi distinti che si possono ottenere sommando tra loro due elementi di a (compreso un elemento sommato a se stesso).

Ad esempio:
	input: 1 2
	output: 2 3 4   [infatti: 2=1+1, 3=1+2, 4=2+2] 
	input: 1 0 0 1 0 0 1 
	output: 2 1 0   [l'ordine nell'array di output non è importante]
	input: -2 3 1 10 
	output: -4 1 -1 8 6 4 13 2 11 20


Dato che non è possibile stabilire a priori quale sara la lunghezza dell'array risultato, 
tale lunghezza deve essere passata per riferimento utilizzando il puntatore *size.

Si scriva poi un programma che invoca somme_distinte passandogli l'array ottenuto convertendo in interi i valori forniti sulla linea di comando 
e successivamente stampa su stderr l'array restituito da somme_distinte. 
Infine il programma deve stampare su stdout la somma dei valori contenuti nell'array restituito da somme_distinte. 
(Nota: per stampare su stdout si può usare fprintf(stdout,...) oppure semplicemente printf). 
Il programma deve deallocare tutta la memoria utilizzata (verificare con valgrind).
*/


// stampa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}

bool presente(int *a, int size, int n){
	for (int i=0; i<size; i++){
		if(a[i]==n) return true;
	}
	return false;
}


int *somme_distinte(int a[], int n, int *size){
	int messi = 0;
	int sizeb = n;
	int *b = malloc(sizeb*sizeof(int));
	if(b==NULL) termina("Malloc di b fallita");

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			int x = a[i] + a[j];
			//printf("Elemento 1: %d, elemento 2: %d, somma=%d\n", a[i], a[j], x);
			if(!presente(b, messi, x)){
				if(sizeb == messi){
					sizeb = 2*sizeb;
					b = realloc(b, sizeb*sizeof(int));
					if(b==NULL) termina("Realloc fallita");
				}
				b[messi] = x;
				messi++;
			}
		} 
	}

	b = realloc(b, messi*sizeof(int));
	*size = messi;

	return b;


}


int main(int argc, char *argv[])
{
	if(argc<2){
		printf("USO: %s i1 12 ...\n", argv[0]);
	}

	int size = argc -1;
	int sizeb;
	int *a = malloc(size*sizeof(int));
	if(a==NULL) termina("Malloc di a fallita");

	for(int i=0; i<size; i++){
		a[i]=atoi(argv[i+1]);
	}

	

	int *b = somme_distinte(a, size, &sizeb);  

	printf("Somme: ");

	for(int i=0; i<sizeb; i++){
		printf("%d ",b[i]);
	}
	puts(" ");

	free(a);
	free(b);

	return 0;
}
