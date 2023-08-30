#include "xerrori.h"

//struttura per il thread
typedef struct {
    int id;
    char *nome;
} dati;

void *body(void *arg) {
    dati *d = (dati *) arg;
    printf("Hello from thread!\n");
    printf("Nome: %s - Id: %d\n", d->nome, d->id);
    d->id++;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        printf("Uso:\n\t%s nome \n", argv[0]);
        exit(1);
    }
    char *nome = argv[1];
    pthread_t t1;
    dati d;
    d.id = 1;
    d.nome = nome;
    pthread_create(&t1, NULL, &body, &d);

    pthread_join(t1, NULL);
    
    printf("Hello from main!\n %d\n", d.id);

    return 0;
}