/*
 * Esempio paradigma produttore consumatori
 * in cui abbiamo più di un produttore e 
 * più di consumatore
 * 
 * I produttori leggono gli interi dai file e li scrivono 
 * nel buffer, i consumatori calcolano il numero di 
 * divisori di ogni intero
 * 
 * */

#include "xerrori.h"


#define QUI __LINE__,__FILE__
#define Buf_size 10


// funzione per contare il numero di divisori (DATA DAL PROF)
int divisori(int n)
{
  assert(n>0);
  printf("%d\n",n);
  int d = 0;
  for (int i=1; i<=n; i ++)
      if(n%i==0) d++;
  return d;
}

// struct contenente i parametri di input 
// per i thread consumatori 
typedef struct {
  int *buffer; 
  int *pcindex;
  pthread_mutex_t *pmutex_buf;
  pthread_mutex_t *pmutex_file;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;
  FILE *outfile;  
} dati_consumatori;

// struct contenente i parametri di input 
// per i thread produttori
typedef struct {
  int *buffer; 
  int *ppindex;
  pthread_mutex_t *pmutex_buf;
  sem_t *sem_free_slots;
  sem_t *sem_data_items;
  char *nomefile;  
} dati_produttori;



// funzione eseguita dai thread consumer
void *cbody(void *arg)
{  
  dati_consumatori *a = (dati_consumatori *)arg; 

  puts("consumatore partito");
  int n;
  do {
    //faccio la wait sul semaforo dei dati
    xsem_wait(a->sem_data_items,__LINE__,__FILE__);
    // LOCK sul buffer
    xpthread_mutex_lock(a->pmutex_buf,QUI);
    n = a->buffer[*(a->pcindex) % Buf_size];
    *(a->pcindex) +=1;
    xpthread_mutex_unlock(a->pmutex_buf,QUI);
    if(n==-1) break;
    xsem_post(a->sem_free_slots,__LINE__,__FILE__);
    int div = divisori(n);
    xpthread_mutex_lock(a->pmutex_file,QUI);
    fprintf(a->outfile,"%d %d\n",n,div);
    xpthread_mutex_unlock(a->pmutex_file,QUI);
  } while(n != -1);
  puts("Consumatore sta per finire");
  pthread_exit(NULL); 
}     

// funzione eseguita dai thread producer
void *pbody(void *arg)
{  
  dati_produttori *a = (dati_produttori *)arg; 

  puts("produttore partito");
  // apre il file e termina se non riesce
  FILE *f = fopen(a->nomefile,"rt");
  if(f==NULL) {
    fprintf(stderr,"Apertura file %s fallita\n",a->nomefile);
    pthread_exit(NULL); 
  }
  int n;
  do {
    int e = fscanf(f,"%d",&n);
    if(e!=1) break;
    xsem_wait(a->sem_free_slots,QUI);
    xpthread_mutex_lock(a->pmutex_buf,QUI);
    a->buffer[*(a->ppindex) % Buf_size] = n;
    *(a->ppindex) +=1;
    xpthread_mutex_unlock(a->pmutex_buf,QUI);
    xsem_post(a->sem_data_items,QUI);
  } while(true);
  puts("produttore sta per finire");
  pthread_exit(NULL); 
}     


int main(int argc, char *argv[])
{
  // leggi input
  if(argc<4) {
    printf("Uso\n\t%s file1 [file2 ...] outfile numt\n", argv[0]);
    exit(1);
  }
  // numero di thread prod e consumatori
  int tp = argc-3; //creo un produttore per ogni file
  int tc = atoi(argv[argc-1]);
  assert(tp>0);
  assert(tc>0);
  //apro il file di output
  FILE* outfile = fopen(argv[argc-2],"wt");
  if(outfile==NULL)
    xtermina("impossibile aprire outfile",QUI);

  // buffer produttori-consumatori
  int buffer[Buf_size];
  int pindex=0, cindex=0;
  
  //mutex per produttori
  pthread_mutex_t mupbuf = PTHREAD_MUTEX_INITIALIZER;
  //mutex per consumatori
  pthread_mutex_t mucbuf = PTHREAD_MUTEX_INITIALIZER;
  //sem per file di output
  pthread_mutex_t mucfile = PTHREAD_MUTEX_INITIALIZER;

  //semafori per il buffer
  sem_t sem_free_slots, sem_data_items;
  xsem_init(&sem_free_slots,0,Buf_size,__LINE__,__FILE__);
  xsem_init(&sem_data_items,0,0,__LINE__,__FILE__);

  // creo la struttura dati per i thread
  dati_produttori ap[tp];
  dati_consumatori ac[tc];
  pthread_t prod[tp];       // id thread produttori
  pthread_t cons[tc];       // id thread consumatori 


  // creo tutti i produttori
  for (int i=0; i<tp; i++){
    ap[i].nomefile = argv[i+1];
    ap[i].ppindex = &pindex;
    ap[i].pmutex_buf = &mupbuf;
    ap[i].sem_free_slots = &sem_free_slots;
    ap[i].sem_data_items = &sem_data_items;
    ap[i].buffer = buffer;
    xpthread_create(&prod[i],NULL,pbody,&ap[i],__LINE__,__FILE__);
  }
  puts("Thread produttori creati");

  // creo tutti i consumatori
  for (int i=0; i<tc; i++){
    ac[i].pcindex = &cindex;
    ac[i].pmutex_buf = &mucbuf;
    ac[i].pmutex_file = &mucfile;
    ac[i].sem_free_slots = &sem_free_slots;
    ac[i].sem_data_items = &sem_data_items;
    ac[i].outfile = outfile;
    ac[i].buffer = buffer;
    xpthread_create(&cons[i],NULL,cbody,&ac[i],__LINE__,__FILE__);
  }
  puts("Thread consumatori creati");

  // attendo i produttori
  for (int i=0; i<tp; i++){
    xpthread_join(prod[i],NULL,__LINE__,__FILE__); 
  }
  // comunico ai consumatori che possono terminare
  for(int i=0;i<tc;i++) {
    xsem_wait(&sem_free_slots,__LINE__,__FILE__);
    buffer[pindex++ % Buf_size] = -1;
    xsem_post(&sem_data_items,__LINE__,__FILE__);
  }
  puts("Valori di terminazione scritti");

  // attendo i consumatori 
  for (int i=0; i<tc; i++){
    xpthread_join(cons[i],NULL,__LINE__,__FILE__);  
  }

  // deallocazione, saluti, etc....
  xsem_destroy(&sem_free_slots,__LINE__,__FILE__);
  xsem_destroy(&sem_data_items,__LINE__,__FILE__);
  xpthread_mutex_destroy(&mupbuf,__LINE__,__FILE__);
  xpthread_mutex_destroy(&mucbuf,__LINE__,__FILE__);
  xpthread_mutex_destroy(&mucfile,__LINE__,__FILE__);
  puts("Thread ausiliari distrutti");

  return 0;
}
