#include "xerrori.h"
#define QUI __LINE__,__FILE__

// esempio uso di segnali in presenza di diversi thread
// su linux il tid può essere usato per inviare
// segnali ai singoli thread dalla linea di comando


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



// variabili globali utilizzate dal main e dal signal handler 
int tot_segnali = 0;
// il perche' della keyword volatile lo vediamo a lezione
volatile bool continua = true;

// funzione che viene invocata quando viene ricevuto 
// un segnale USR1 USR2 o INT (Control-C)
void handler(int s)
{
  tot_segnali++;
  printf("Segnale %d ricevuto dal processo %d tid:%d\n", s,getpid(),gettid());
  if(s==SIGUSR2) {
    kill(getpid(),SIGUSR1);
    continua = false;
  } 
}

void *tbody(void *v) {
	while(true) {
		sleep(1);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
  // definisce signal handler 
  struct sigaction sa;
  sa.sa_handler = handler;
	// setta a "insieme vuoto" sa.sa_mask che è la
	// maschera di segnali da bloccare: non una buona idea  
  sigemptyset(&sa.sa_mask);     
  sa.sa_flags = SA_RESTART;     // restart system calls if interrupted
  sigaction(SIGUSR1,&sa,NULL);  // handler per USR1
  sigaction(SIGUSR2,&sa,NULL);  // handler per USR2
  sigaction(SIGINT,&sa,NULL);   // handler per Control-C

  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());
	pthread_t t[2];
	xpthread_create(&t[0],NULL,tbody,NULL,QUI);
	xpthread_create(&t[1],NULL,tbody,NULL,QUI);

  // mandiamo segnali ai thread 
	sleep(2);
	pthread_kill(t[0],SIGUSR1);
	pthread_kill(t[1],SIGUSR1);
	// mando un sigint al processo 
	kill(getpid(),SIGINT);
  // ora decido che voglio ignorare control-C
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask,SIGINT);
	pthread_sigmask(SIG_BLOCK,&mask,NULL);
	printf("Segnale %d ignorato dal tid:%d\n",SIGINT,gettid());
	
	
  // entra in orribile loop attendendo i segnali
  continua = true;
  do { // loop apparentemente senza uscita
    sleep(1);               
  } while(continua);
  printf("Ricevuti: %d segnali\n", tot_segnali);   
  return 0;
}

