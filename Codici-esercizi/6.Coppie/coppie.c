#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>


/*
Si consideri la struct

     typedef struct cop {
       int primo;
       int secondo;
       struct cop *next; 
     } coppia;

che permette di rappresentare una lista di coppie di interi.

1) Scrivere un file coppie.c contenente le seguenti funzioni (più eventuali altre che ritenete utili). 
Nel seguito lis è utilizzata per indicare una variabile che rappresenta una lista di coppie (che puo' essere vuota).


// crea una coppia contenente i valori x e y
coppia *crea_coppia(int x, int y)

Legge 2 interi dal file f (usando scanf) e crea una coppia con i valori corrispondenti chiamando la funzione crea_coppia(). 
Restituisce NULL se non e' possibile leggere i due interi dal file f*
coppia *crea_coppia_file_testo(FILE *f)

//Crea una lista con tutti gli interi presenti nel file il cui nome e' nomefile. 
Gli interi devono essere letti con crea_coppie_file_testo(). 
Nella lista restituita le coppie devono essere ordinate per somma delle componenti crescenti
coppia *crea_lista_ordinata(char *nomefile)

// stampa sul file f tutte le coppie di lis 
// !!! NON MODIFICATE QUESTA FUNZIONE!!!
void stampa_lista_coppie(coppia *lis, FILE *f) {
  while(lis!=NULL) {
    fprintf(f,"%d %d\n",lis->primo, lis->secondo);
    lis = lis->next;
  }
}

// dealloca tutti gli elementi della lista lis
void distruggi_lista_coppie(coppia *lis)

2) Scrivere una funzione

    coppia *cerca_coppia(coppia *lis, bool (*f)(int, int))

che data una lista di coppie lis restituisce il puntatore all'elemento più lontano dalla testa della lista per cui la funzione f applicata alle componenti della coppia restituisce true. Ad esempio se lis contiene le coppie (1,2) (5,-1) (8,-3) (4,4) (8,2) (3,9) (20,-2) la funzione cerca_coppia con secondo argomento la funzione minore() definita in main.c deve restituire il puntatore all'elemento (3,9) (perché tra gli elementi della lista per cui minore vale true (3,9) è quello più lontano dalla testa della lista). Se la funzione f applicata alle componenti della lista non restituisce mai true la funzione deve restituire NULL. NOTA: la funzione deve restituire il puntatore all'elemento richiesto, senza modificare la lista.

Per testare il vostro programma potete usare il main fornito scommentando le chiamate alle funzioni mano a mano che le scrivete. Potete anche usare i file in0 e in1 che se passati come argomento sulla linea di comando nel seguente modo:

./main in0 > out0
./main in1 > out1

dovrebbero produrre rispettivamente gli output out0 e out1 (ricordate che potete usare stderr per visualizzare sul terminale informazioni di debug). Se non modificate la funzione stampa_lista_coppie i vostri file dovrebbero essere identici a out0 e out1 e li potete confrontare dalla linea di comando con diff.

*/

typedef struct cop {
  int primo;
  int secondo;
  struct cop *next; 
} coppia;

// stampa un messaggio d'errore e termina il programma
void termina(const char *messaggio);


// crea una coppia contenente i valori x e y
coppia *crea_coppia(int x, int y){
  coppia *c = malloc(sizeof(*c));
  c->primo = x;
  c->secondo = y;
  c->next = NULL;
  return c;
}



/*Legge 2 interi dal file f (usando scanf) e crea una coppia con i valori corrispondenti chiamando la funzione crea_coppia(). 
Restituisce NULL se non e' possibile leggere i due interi dal file f*/
coppia *crea_coppia_file_testo(FILE *f){
  assert(f!=NULL);
  int primo;
  int secondo;
  int e = fscanf(f, "%d %d", &primo, &secondo);
  if(e!=2)
    return NULL;
  coppia *c = crea_coppia(primo, secondo); 
  return c;
}


//funzione per ordinare
int cmp(coppia *a, coppia *b){
  int somma1 = (a->primo) + (a->secondo);
  int somma2 = (b->primo) + (b->secondo);
  if(somma1 <= somma2) return 0;
  return 1;
}

coppia *inserisci(coppia *testa, coppia *c){
  //lista vuota
  if(testa == NULL){
    c->next = NULL;
    return c;
  }
  if ((c->primo + c->secondo)<=(testa->primo + testa->secondo)){
    c->next = testa;
    return c;
  }
  testa->next = inserisci(testa->next, c);
  return testa;

}

/*Crea una lista con tutti gli interi presenti nel file il cui nome e' nomefile. 
Gli interi devono essere letti con crea_coppie_file_testo(). 
Nella lista restituita le coppie devono essere ordinate per somma delle componenti crescenti*/
coppia *crea_lista_ordinata(char *nomefile){
  FILE *f = fopen(nomefile, "r");
  if(f==NULL) termina("Errore nell'apertura del file");
  coppia *testa = NULL;
  while(true){
    coppia *c = crea_coppia_file_testo(f);
    if(c==NULL) break;
    testa = inserisci(testa, c);
  }
  fclose(f);
  return testa;
}

// stampa sul file f tutte le coppie di lis 
// !!! NON MODIFICATE QUESTA FUNZIONE!!!
void stampa_lista_coppie(coppia *lis, FILE *f) {
  while(lis!=NULL) {
    fprintf(f,"%d %d\n",lis->primo, lis->secondo);
    lis = lis->next;
  }
}

// dealloca tutti gli elementi della lista lis
void distruggi_lista_coppie(coppia *lis){
  while(lis->next != NULL){
    coppia *temp = lis->next;
    free(lis);
    lis = temp;
  }
  free(lis);
}

//data una lista di coppie lis restituisce il puntatore all'elemento più lontano dalla testa della lista
//per cui la funzione f applicata alle componenti della coppia restituisce true. 
//Ad esempio se lis contiene le coppie 
//   (1,2) (5,-1) (8,-3) (4,4) (8,2) (3,9) (20,-2) 
//la funzione cerca_coppia con secondo argomento la funzione minore() definita in main.c 
//deve restituire il puntatore all'elemento (3,9) 
//(perché tra gli elementi della lista per cui minore vale true (3,9) è quello più lontano dalla testa della lista). 
//Se la funzione f applicata alle componenti della lista non restituisce mai true la funzione deve restituire NULL. 
//NOTA: la funzione deve restituire il puntatore all'elemento richiesto, senza modificare la lista.
coppia *cerca_coppia(coppia *lis, bool (*f)(int, int)){
  bool trovata = false;
  coppia *c = lis;
  while(lis->next != NULL){
    if(f(lis->next->primo, lis->next->secondo)){
      trovata = true;
      c = lis->next;
    }
    lis = lis->next;
  }
  if(trovata == false) return NULL;
  return c;
}


//queste funzioni servono per testare cerca_coppia
bool minore(int a, int b)
{
  return a<b;
}
bool pari(int a, int b)
{
  return (a%2==0) && (b%2==0);
}



int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Uso: %s nomefile\n",argv[0]);
        return 1;
    }
		puts("==== inizio");
    coppia *lista = crea_lista_ordinata(argv[1]);
    stampa_lista_coppie(lista,stdout);
    puts("---- 1");
    coppia *lista2 = cerca_coppia(lista,minore);
    stampa_lista_coppie(lista2,stdout);
    puts("---- 2");
    lista2 = cerca_coppia(lista,pari);
    stampa_lista_coppie(lista2,stdout);		
		puts("---- 3");
    distruggi_lista_coppie(lista);
    distruggi_lista_coppie(lista2);
		puts("==== fine");

    return 0;
}




void termina(const char *messaggio){
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}


