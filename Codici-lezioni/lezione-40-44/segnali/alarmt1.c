#include "xerrori.h"
#define QUI __LINE__,__FILE__

// esempio di blocco di tutti i segnali 
// gestione di SIGALRM con sigwait 


// ridefinizione funzione gettid() (non è sempre disponibile) 
#include <sys/syscall.h>   /* For SYS_xxx definitions */
pid_t gettid(void)
{
	#ifdef __linux__
	  // il tid è specifico di linux
    pid_t tid = (pid_t)syscall(SYS_gettid);
    return tid;
  #else
	  // per altri OS restituisco -1
	  return -1;
  #endif
}


// il perche' della keyword volatile lo abbiamo già visto
volatile sig_atomic_t continua =1;
// usando sigwait questa funzione non viene più eseguita
void gestore(int s)
{
  char b[3];
  b[0] = '0' + s/10;
  b[1] = '0' + s%10;
  b[2] = '\n';
  int e = write(1,"Segnale ",8);
  e = write(1,b,3);
  (void) e;
  continua = 0;
}

// thread nullafacente
void *tbody(void *v) {
  while(true) {
    pause();
    printf("====== Thread :%d svegliato\n",gettid());
  }
  return NULL;
}

// thread che effettua la gestione di tutti i segnali
void *tgestore(void *v) {
  sigset_t mask;
  sigfillset(&mask);
  int s;
  while(true) {
    int e = sigwait(&mask,&s);
    if(e!=0) perror("Errore sigwait");
    printf("Thread gestore %d svegliato dal segnale %d\n",gettid(),s);
  }
  return NULL;
}


int main (void) {
  // definisce signal handler per SIGALARM
  struct sigaction sa;
  sigaction(SIGALRM,NULL,&sa); // copio sigaction corrente
  sa.sa_handler = gestore;     // definisco funzione di gestione
  sigaction(SIGALRM,&sa,NULL); // assegno sigaction
  
  // blocco i segnali
  sigset_t mask;
  sigfillset(&mask);  // insieme di tutti i segnali
  sigdelset(&mask,SIGQUIT); // elimino sigquit
  pthread_sigmask(SIG_BLOCK,&mask,NULL); // blocco tutto tranne sigquit
  
  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());
  pthread_t t[3];
  xpthread_create(&t[0],NULL,tbody,NULL,QUI);
  xpthread_create(&t[1],NULL,tbody,NULL,QUI);
  // lancio il thread gestore di segnali
  xpthread_create(&t[2],NULL,tgestore,NULL,QUI);
  
  
  while(true) {
    // SIGALRM fra 2 secondi
    alarm(2);
    puts("Inizio ciclo infinito");
    continua=1;
    while (continua) 
      sleep(1);
    puts("Uscito dal ciclo");
  }
  return 0 ;
}

  
