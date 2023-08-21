#include "xerrori.h"

// macro per indicare la posiine corrente
#define QUI __LINE__,__FILE__

// dimensione buffer produttori-consumatori
#define Buf_size 20


// calcola gcd di due interi >= 0 non entrambi nulli 
int gcd(int a, int b)
{
	assert(a>=0 && b>=0);
	assert(a!=0 || b!=0);
  if(b==0) return a;
  return gcd(b,a%b);
}

typedef struct{
  int *buffer; // buffer produttori-consumatori
  int *ppindex; // indice per la scrittura nel buffer
  pthread_mutex_t *pmutex_buf; // mutex per il buffer
  pthread_mutex_t *pmutex_file; // mutex per il file
  sem_t *sem_free_slots; // semaforo slot liberi nel buffer
  sem_t *sem_data_items; // semaforo dati nel buffer
  FILE *infile;
}data_produttori;

void *pbody (void *arg){
  data_produttori *a = (data_produttori *)arg;
  puts("Produttore partito");
  int n, m, d;
  int t=0;
  do{
    //lock mutex file
    xpthread_mutex_lock(a->pmutex_file,QUI);
    //leggo la coppia dal file 
    int e = fscanf(a->infile,"%d %d", &n, &m);
    t +=1;
    printf("n:%d m:%d     t:%d\n",n,m,t);
    if(e!=2) break;
    //unlock mutex file
    xpthread_mutex_unlock(a->pmutex_file,QUI);
    //calcolo gcd
    d = gcd(n,m);
    
    //wait su sem_free_slots
    xsem_wait(a->sem_free_slots,__LINE__,__FILE__);
    //lock mutex buffer
    xpthread_mutex_lock(a->pmutex_buf,QUI);
    //scrivo d nel buffer
    a->buffer[*(a->ppindex) % Buf_size] = d;
    //incremento ppindex
    *(a->ppindex) +=1;
    //unlock mutex buffer
    xpthread_mutex_unlock(a->pmutex_buf,QUI);
    //post su sem_data_items
    xsem_post(a->sem_data_items,__LINE__,__FILE__);
  } while(true);
  puts("Produttore sta per finire");
  pthread_exit(NULL);

  

}

int cmp(int *a, int *b){
  if(*a<*b) return -1;
  if(*a>*b) return 1;
  return 0;
}


int main(int argc, char *argv[])
{
  // controlla numero argomenti
  if(argc!=3) {
      printf("Uso: %s file numT\n",argv[0]);
      return 1;
  }
  // apre file di input
  FILE *infile = fopen(argv[1],"rt");
  if(infile==NULL) {
    fprintf(stderr,"Apertura file %s fallita\n",argv[1]);
    return 1;
  }
  // numero di thread produttori
  int tp = atoi(argv[2]);
  assert(tp>0);

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

  data_produttori ap[tp];
  pthread_t pr[tp];

  //creo i produttori
  for(int i=0; i<tp; i++){
    ap[i].ppindex = &pindex;
    ap[i].pmutex_buf = &mupbuf;
    ap[i].pmutex_file = &mucfile;
    ap[i].sem_free_slots = &sem_free_slots;
    ap[i].sem_data_items = &sem_data_items;
    ap[i].infile = infile;
    ap[i].buffer = buffer;
    pthread_create(&pr[i],NULL,pbody,(void *)&ap[i]);
  }
  puts("Produttori creati");
  int *arrayfinanale;
  int size = Buf_size;
  int messi = 0;
  arrayfinanale = malloc(size*sizeof(int));
  if(arrayfinanale==NULL) {
    xtermina("Errore allocazione memoria", __LINE__, __FILE__);
  }
  int n;
  //unico thread consumatore:
  do{
    //prendo mutex per il buffer
    xpthread_mutex_lock(&mucbuf,__LINE__,__FILE__);
    //wait su sem_data_items
    xsem_wait(&sem_data_items,__LINE__,__FILE__);
    n = buffer[cindex % Buf_size];
    cindex +=1;
    //rilascio la mutex per il buffer
    xpthread_mutex_unlock(&mucbuf,__LINE__,__FILE__);
    //post su sem_free_slots
    xsem_post(&sem_free_slots,__LINE__,__FILE__);
    if(n==-1) break;
    if(messi == size){
      size *= 2;
      arrayfinanale = realloc(arrayfinanale,size*sizeof(int));
      if(arrayfinanale==NULL) {
        xtermina("Errore allocazione memoria", __LINE__, __FILE__);
      }
    }
    arrayfinanale[messi] = n;
    messi +=1;
  }while(n!=-1);

  //attendo i produttori
  for(int i=0; i<tp; i++){
    xpthread_join(pr[i],NULL,__LINE__,__FILE__);  
  }
  //comunico ai consumatori che possono terminare
  xsem_wait(&sem_free_slots,__LINE__,__FILE__);
  buffer[pindex++ % Buf_size] = -1;
  xsem_post(&sem_data_items,__LINE__,__FILE__);
  
  //realloco array finale
  arrayfinanale = realloc(arrayfinanale,messi*sizeof(int));
  if(arrayfinanale==NULL) {
    xtermina("Errore allocazione memoria", __LINE__, __FILE__);
  }
  
  //ordino l'array con la funzione qsort
  qsort(arrayfinanale,messi,sizeof(int),(__compar_fn_t) &cmp);

  //stampo l'array
  for(int i=0; i<messi; i++){
    fprintf(stdout, "%d ",arrayfinanale[i]);
  }
  printf("\n");

  fclose(infile);
  free(arrayfinanale);
  pthread_exit(NULL);

  return 0;
}
