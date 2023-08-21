//provo a rifare l'esercizio produttori consumatori

#include "xerrori.h"

#define QUI __LINE__,__FILE__
#define Buf_size 10

// funzione per stabilire se n è primo  
bool primo(int n)
{
    if(n<2) return false;
    if(n%2==0) return (n==2);
    for (int i=3; i*i<=n; i += 2)
            if(n%i==0) return false;
    return true;
}

//struct contenente i parametri di input e output di ogni thread 
typedef struct {
    int quanti;              // quanti numeri sono stati analizzati 
    long somma;              // somma di tutti i numeri
    int *buffer;             // buffer condiviso dai thread
    int *pcindex;            // indice per la lettura del buffer 
    pthread_mutex_t *pmutex; // mutex condivisa
    sem_t *sem_free_slots;   // semaforo slot liberi nel buffer
    sem_t *sem_data_items;   // semaforo dati nel buffer
} dati;

// funzione eseguita dai thread consumer
void *tbody(void *arg)
{   
    //recupero i parametri
    dati *a = (dati *)arg;
    //setto a zero sia la somma che il numero di primi trovati  
    a->quanti = 0;
    a->somma = 0;
    // n indica il numero letto dal buffer
    int n; 

    puts("consumatore partito");
    
    
    do {
        //faccio la wait sul semaforo dei dati
        xsem_wait(a->sem_data_items,__LINE__,__FILE__);
        // acquisto la mutex condivisa
        xpthread_mutex_lock(a->pmutex,QUI);
        // leggo un numero dal buffer (% Buf_size perche' il buffer ha dimensione Buf_size e lo considero circolare)
        n = a->buffer[*(a->pcindex) % Buf_size];
        // incremento l'indice
        *(a->pcindex) +=1;
        //rilascio la mutex condivisa
        xpthread_mutex_unlock(a->pmutex,QUI);
        //faccio la post sul semaforo dei slot (si è liberato un posto)
        xsem_post(a->sem_free_slots,__LINE__,__FILE__);
        //controllo se il numero è primo e se lo è aggiungo alla somma e incremento il numero di primi
        if(n>0 && primo(n)) {
            a->quanti++;
            a->somma += n;
        }
    } while(n != -1);
    puts("Consumatore sta per finire");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    //leggo input
    if(argc!=2) {
        printf("Uso\n\t%s file\n", argv[0]);
        exit(1);
    }

    //numero di threads
    int p = 3;
    assert(p>=0);

    //inizializzo i dati 
    int tot_primi = 0;
    long tot_somma = 0;
    // e = per gli errori, n = per gli interi dal file, cindex = per la lettura del buffer
    int e, n = 0;

    //indici per il buffer
    int pindex = 0;
    int cindex = 0;

    //Dati per i thread 
    int buffer[Buf_size];
    
    pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    pthread_t t[p];
    dati a[p];
    sem_t sem_free_slots, sem_data_items;
    xsem_init(&sem_free_slots,0,Buf_size,__LINE__,__FILE__);
    xsem_init(&sem_data_items,0,0,__LINE__,__FILE__);

    //creo i thread
    for(int i=0;i<p;i++) {
        // faccio partire il thread i
        a[i].buffer = buffer;
        a[i].pcindex = &cindex;
        a[i].pmutex = &mu;
        a[i].sem_data_items = &sem_data_items;
        a[i].sem_free_slots = &sem_free_slots;
        xpthread_create(&t[i],NULL,tbody,a+i,__LINE__,__FILE__);
    }
    puts("Thread ausiliari creati");

    //PRODUTTORE

    //leggo il file dei numeri da analizzare
    FILE* file = fopen(argv[1],"r");
    if(file==NULL){
        perror("Errore apertura file");
        return 1;
    }

    // ciclo di lettura dal file
    while(true) {
        e = fscanf(file,"%d", &n);
        if(e!=1) break; // se il valore e' letto correttamente e==1
        assert(n>0);    // i valori del file devono essere positivi
        xsem_wait(&sem_free_slots,__LINE__,__FILE__);
        buffer[pindex++ % Buf_size]= n;
        xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    puts("Dati del file scritti");

    //terminazione dei threads
    for(int i=0;i<p;i++) {
        xsem_wait(&sem_free_slots,__LINE__,__FILE__);
        buffer[pindex++ % Buf_size]= -1;
        xsem_post(&sem_data_items,__LINE__,__FILE__);
    }
    puts("Valori di terminazione scritti");

    // join dei thread e calcolo risulato
    for(int i=0;i<p;i++) {
        xpthread_join(t[i],NULL,__LINE__,__FILE__);
        tot_primi += a[i].quanti;
        tot_somma += a[i].somma;
    }
    pthread_mutex_destroy(&mu);
    fprintf(stderr,"Trovati %d primi con somma %ld\n",tot_primi,tot_somma);


    return 0;
}
