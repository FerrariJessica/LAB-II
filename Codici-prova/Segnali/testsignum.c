#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni di confronto/copia/etc di stringhe
#include <errno.h>    // richiesto per usare errn

//per usare i segnali
#include <signal.h>

/* un gestore piuttosto semplice */
static void gestore (int signum) {
	printf("Ricevuto segnale %d\n",signum);
	exit(EXIT_FAILURE);
}


/* genero una sequenza infinita di interi */
int main (void) {
	struct sigaction s; int i;
	/* inizializzo s a 0*/
	memset( &s, 0, sizeof(s) );
	s.sa_handler=gestore; /* registro gestore */
	/* installo nuovo gestore s */
	ec_meno1( sigaction(SIGINT,&s,NULL) );
	for (i=1; ;i++) { /* ciclo infinito */
		sleep(1);
		printf("%d \n",i);
	}
	exit(EXIT_SUCCESS); /* mai eseguita */
} 
