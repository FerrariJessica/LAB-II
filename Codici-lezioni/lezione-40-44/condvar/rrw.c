#include "xerrori.h"
#define QUI __LINE__,__FILE__


// Possibile soluzione al problema lettori/scrittori
// Questa soluzione è un po' più equa per gli scrittori che prima
// erano penalizzati

typedef struct {
  int readers;
	int waiting_w;         // TENIAMO CONTO DEGLI SCRITTORI IN ATTESA
	// IDEA: se c'è uno scrittre in attesa e qualcuno vuole leggere non 
	// gli diamo l'accesso
	bool writing;
  pthread_cond_t condW;   // condition variable
  pthread_cond_t condR;   // condition variable lettori
  // perché con una cv unica non posso decidere che gruppo svegliare
  pthread_mutex_t mutex; // mutex associato alla condition variable
} rw;

// inzializza semaforo al valore q
void rw_init(rw *z) {
  z->readers = 0;
	z->waiting_w = 0;
	z->writing = false;
  xpthread_cond_init(&z->condW,NULL,QUI);
	xpthread_cond_init(&z->condR,NULL,QUI);
  xpthread_mutex_init(&z->mutex,NULL,QUI);
}

void read_lock(rw *z) {
  pthread_mutex_lock(&z->mutex);
  while(z->writing==true || z->waiting_w>0) // se ho dei scrittori in attesa aspetto
    pthread_cond_wait(&z->condR, &z->mutex);   // attende fine scrittura
  z->readers++;
  pthread_mutex_unlock(&z->mutex);
}


void read_unlock(rw *z) {
	assert(z->readers>0);
  pthread_mutex_lock(&z->mutex);
  z->readers--;                  // cambio di stato       
  if(z->readers==0) 
		pthread_cond_signal(&z->condW); // da segnalare ad un solo writer
  pthread_mutex_unlock(&z->mutex);
}
  
void write_lock(rw *z) {
  pthread_mutex_lock(&z->mutex);
	z->waiting_w++;
  while(z->writing || z->readers>0)
		// attende fine scrittura o lettura
    pthread_cond_wait(&z->condW, &z->mutex);   
  z->writing = true;
	z->waiting_w--;
  pthread_mutex_unlock(&z->mutex);
}

void write_unlock(rw *z) {
	assert(z->writing);
  pthread_mutex_lock(&z->mutex);
  z->writing=false;               // cambio stato
	// segnala a tutti quelli in attesa 
	if(z->waiting_w > 0) // se qualchuno sta aspettando sveglio lo scrittore
		pthread_cond_signal(&z->condW); 
  else
    pthread_cond_broadcast(&z->condR); 
  pthread_mutex_unlock(&z->mutex);
}