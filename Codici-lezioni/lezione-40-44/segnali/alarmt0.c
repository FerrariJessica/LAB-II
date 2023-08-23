#include "xerrori.h"
#define QUI __LINE__,__FILE__


// esempio di uso di alarm() in presenza anche di altri thread 
	

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


// il perche' della keyword volatile lo vediamo a lezione
volatile sig_atomic_t continua = 1;
void gestore(int s)
{
  char b[3];
  b[0] = '0' + s/10;
  b[1] = '0' + s%10;
  b[2] = '\n';
  int e = write(1,"Segnale ",8);
  e = write(1,b,3);
  (void) e; // evita warning variabile non usata
  continua = 0;
}

// thread nullafacente
void *tbody(void *v) {
  while(true) {
    pause();
    printf("======= Thread :%d svegliato\n",gettid());
  }
  return NULL;
}

int main (void) {
  // definisce signal handler 
  struct sigaction sa;
  sigaction(SIGALRM,NULL,&sa); // inizializzo sa
  sa.sa_handler = gestore;
  sigaction(SIGALRM,&sa,NULL); // inizializzo sa  
  
  // visualizza il pid
  printf("Se vuoi mandarmi dei segnali il mio pid e': %d\n", getpid());
  pthread_t t[2];
  xpthread_create(&t[0],NULL,tbody,NULL,QUI);
  xpthread_create(&t[1],NULL,tbody,NULL,QUI);
  
  while(true) {
    //  SIGALRM fra 3 secondi
    alarm(3);
    puts("Inizio ciclo infinito");
    continua=1;
    while (continua) 
      sleep(1);
    puts("Uscito dal ciclo");
  }
  return 0 ;
}
