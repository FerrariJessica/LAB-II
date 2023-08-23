#include "xerrori.h"
// abbreviamo la chiamata alle funzioni x...
#define QUI __LINE__, __FILE__

// esempio funzionamento funzione alarm()
// con e senza handler

// il perche' della keyword volatile lo vediamo a lezione
volatile sig_atomic_t continua = 1;

void gestore(int s) {
  char b[3];
  b[0] = '0' + s / 10;
  b[1] = '0' + s % 10;
  b[2] = '\n';
  int e = write(1,"Segnale ",8);
  e = write(1,b,3);
  (void) e; // evita warning variabile non usata
  continua = 0;
}

int main(void) {
  // definisce signal handler
  struct sigaction sa;
  sigaction(SIGALRM, NULL, &sa); // inizializzo sa
  sa.sa_handler = gestore;       // fornisce handler
  sigaction(SIGALRM, &sa, NULL); // usa gestore per il segnale senza cambiare altro 
  //  SIGALRM fra 3 secondi
  alarm(3);
  puts("Inizio ciclo infinito");
  while (continua)
    sleep(1);
  printf("Uscito dal ciclo\n");
  return 0;
}
