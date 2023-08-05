#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno

// prototipi delle funzioni che appaiono dopo il main()
void termina(const char *messaggio);
void stampa_array_stringhe(char *a[], int n,FILE *f);


// esegue il merge di due array di stringhe
void merge(char *a[], int na, char *c[], int nc, char *b[]) 
{
  assert(a!=NULL && c!=NULL && b !=NULL);
  int n = na+nc;  // lunghezza array risultato
  int ia,ib,ic;   // indici all'interno degli array
  ia=ic=ib=0;
  
  // eseguo merge riempiendo il vettore b
  for(ib=0;ib<n;ib++) {
    if(ia==na)
      b[ib] = c[ic++];
    else if(ic==nc)
      b[ib] = a[ia++];    
    else if(strcmp(a[ia],c[ic])<0)
      b[ib] = a[ia++];
    else 
      b[ib] = c[ic++];
  }
  // verifica tutti gli indici sono arrivati in fondo
  assert(ia==na);
  assert(ic==nc);
  assert(ib==n);
}


// ordina un array di stringhe con il mergesort
void mergesort(char *a[], int n)
{
  assert(a!=NULL);
  assert(n>0);
  
  // caso base
  if(n==1) return;
  
  int n1 = n/2;     // dimensione prima parte
  int n2 = n - n1;  // dimensione seconda parte
  
  mergesort(a,n1);
  mergesort(&a[n1],n2); // &a[n1] potevo scriverlo a+n1
  
  // ho le due metà ordinate devo fare il merge
  char **b = malloc(n*sizeof(*b));
  if(b==NULL) termina("malloc fallita nel merge");
  merge(a,n1,&a[n1],n2,b);  
  // copio il risultato da b[] ad a[]
  for(int i=0;i<n;i++)
    a[i] = b[i];
  
  free(b);
}



// il tipo restituito è array di stringhe, di solito lo abbiamo
// scritto char *a[] ma questa notazione non si può usare in questo 
// contesto quindi devo scrivere char ** 
char **leggi_stringhe(FILE *f, int *num)
{
  assert(f!=NULL);
  int size=10; // dimensione attuale dell'array
  int messi=0; // numero di elementi attualmente nell'array
  char **a = malloc(size*sizeof(char *));
  if(a==NULL)
    termina("Memoria insufficiente");
    
  while(true) {
    char *b;
    int e = fscanf(f,"%ms",&b);
    if(e==EOF) break;

    if(messi==size) {
        // ingrandisco l'array
        size = size*2;
        a = realloc(a,size*sizeof(char *));
        if(a==NULL)
          termina("realloc fallita");
    }
    assert(messi<size);
    a[messi] = b;
    messi += 1;
  }
  // ho messo tutti gli elementi che mi interessavano
  size = messi;
  a = realloc(a,size*sizeof(char *));
  if(a==NULL)
    termina("realloc fallita");
  
  // salvo il numero di elementi e restituisco l'array  
  *num = messi;
  return a;  
}


// ordina le stringhe in un file 
int main(int argc, char *argv[])
{


  if(argc!=2) {
    fprintf(stderr, "Uso:\n\t   %s nome_file_di_stringhe\n", argv[0]);
    exit(1);
  }

  FILE *f = fopen(argv[1],"rt");
  if(f==NULL) 
    termina("Errore apertura file");
  // copia le stringhe nell'array
  int n;
  char **a = leggi_stringhe(f,&n);
  
  if(fclose(f)!=0) termina("Errore chiusura file");
  if (a == NULL) 
    termina("lettura fallita");

  // esegue il sorting 
  mergesort(a,n);
  
  // stampo array e chiudo il file
  stampa_array_stringhe(a,n,stdout);
  
  // dealloco le singole stringhe, l'array e termino
  for(int i=0;i<n;i++)
    free(a[i]); // stringhe allocate da scanf
  free(a);
    
  return 0;
}
 

// stampa le stringhe in un array su file *f
void stampa_array_stringhe(char *a[], int n,FILE *f)
{
  assert(f!=NULL);
  assert(a!=NULL);
  for(int i=0;i<n;i++)
    fprintf(f,"%s\n",a[i]); // stampo gli elementi in un campo di 8 caratteri         
}


// stampa su stderr il messaggio che gli passo
// se errno!=0 stampa anche il messaggio d'errore associato 
// a errno. dopo queste stampe termina il programma
// l'argomento è una stringa costante perchè non devo modificarlo
void termina(const char *messaggio)
{
  if(errno==0) 
     fprintf(stderr,"%s\n",messaggio);
  else 
    perror(messaggio);
  exit(1);
}
