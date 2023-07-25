//  Leggere un intero N da tastiera
//  Crea un array *dinamico* con i primi <=N

// prototipo della funzione primo()
// bool primo(int k);
// non necessario in quanto la definizione è prima del main


#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe



// prototipo della funzione primo()
// bool primo(int k);
// non necessario in quanto la definizione è prima del main


// Dato k restituisco True se è primo, False altrimenti
// estremamente inefficiente!!!
bool primo(int k)
{
  for(int i=2; i<k; i++)
    if(k%i == 0) return false; // ho scoperto che il numero non è primo
  return true;
}


// tanpa un messaggio d'errore e termina il programma
void termina(char *messaggio)
{
  puts(messaggio);
  exit(1);
}



int main(int argc, char *argv[])
{
  int n; 
 
  printf("Inserisci il valore N: ");
  int e = scanf("%d",&n); 

  if(e!=1) termina("Valore non trovato");
  if(n<2)  termina("Non ci sono numeri primi");
  
  
  // crea un array dinamico inizialmente di 10 elementi
  int *a;  // dichiaro che a sarà usata come array
  int size = 10;   // dimensione attuale dell'array
  int messi = 0;   // numero di elementi attualmente dentro l'array, lo si utilizza nella realloc

  a  = (int*) malloc(size*sizeof(int));
  
  if(a==NULL) 
    termina("Malloc fallita");
  
  // riempio array
  for(int i=2; i<=n; i++) {
    if(primo(i)) {
      // se l'intero i è primo lo inserisco nella tabella dei primi verificando prima che ci sia spazio
      if(messi == size) {
        size = 2*size; // se la tabella è piena raddoppio la dimensione
        a = (int*) realloc(a, size*sizeof(int));
        if(a == NULL)
          termina("Realloc fallita");
      }
      // inserisco il primo i dentro a[]
      a[messi] = i;
      messi += 1;
    }
  }

  // riduco array alla dimensione minima
  a = (int*) realloc(a, messi*sizeof(int));
  if(a == NULL) termina("Realloc fallita");
    
  // stampa contenuto array, usando 8 caratteri per itero 
  printf("i numeri primi minori di %d sono : ", n );
  for(int i=0; i<=messi; i++) {     
    printf("%8d",a[i]);                    
  }

  printf("\n");
  // dealloco la memoria della tabella
  free(a);
  return 0;
}




