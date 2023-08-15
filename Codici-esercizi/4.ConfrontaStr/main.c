#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>

/*
TESTO:

    Scrivere una funzione
        bool stringhe_uguali(char s[], char q[])
    che date due stringhe (sequenze di caratteri terminate dal carattere \0 ) 
    restiuisce true se le stringhe sono uguali (carattere per carattere) e false altrimenti.

    Nel realizzare la funzione tenete conto che non dovete mai leggere caratteri oltre lo \0 finale di ogni stringa, 
    e che le due stringhe potrebbero avere lunghezze diverse.
    
    Scrivete prima una versione "semplice" che usa la notazione solita degli array (quindi s[i] e q[i]) 
    e se questa funziona una versione che usa l'artimetica dei puntatori (cioè non usate la notazione [] ma accede ai caratteri utilizzando * come abbiamo fatto per il calcolo della lunghezza)
    
    Non modificate il main e per testare il vostro programma scrivete le due stringhe sulla linea di comando.
*/


// stampa un messaggio d'errore e termina il programma
void termina(const char *messaggio);

// scrivere questa funzione
bool stringhe_uguali(char s[], char q[])
{
    int i = 0;
    while(s[i]!='\0' || q[i]!='\0'){
       if(s[i] != q[i]) return false;
       i++;
    }
	return true;
}


// non modificate il main!
int main(int argc, char *argv[])
{
  
  if(argc!=3) {
		fprintf(stderr,"Uso: %s stringa1 stringa2\n",argv[0]);
		exit(1);
	}

	if(stringhe_uguali(argv[1], argv[2]))
	  puts("Uguali!");
	else 
	  puts("Diverse!");

	return 0;
}

void termina(const char *messaggio)
{
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}


