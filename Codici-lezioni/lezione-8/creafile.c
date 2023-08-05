#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe
#include <errno.h>    // rischiesto per usare errno


// stampa un messaggio d'errore su stderr e termina il programma
void termina(char *messaggio)
{
  // se errno!=0 oltre al mio messaggio stampa il messaggio
  // associato alla variabile globale errno 
  // utilizzando la funzione di libreria perror()
  if(errno!=0) perror(messaggio);
  // altrimenti stampa solo il mio messaggio
  else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}



int main(int argc, char *argv[])
{
  int a=1;
  char *nome;
  
  puts("Inserisci il nome e il numero di file:"); 
  int e = scanf("%ms %d",&nome,&a);
  if(e!=2) termina("Errore scanf");


  // genera i nomi e scrive i file
  for(int i=0;i<a;i++) {
    char *s=NULL;
    e = asprintf(&s, "%s.%d", nome, i);
    if(e<0) termina("Errore asprintf");
    FILE *f = fopen(s,"wt");
    if(f==NULL) termina("Errore apertura");
    fprintf(f,"%d\n",i);
    fclose(f);
    free(s);  //  dealloca stringa allocata da asprintf
  }
  return 0;
}

