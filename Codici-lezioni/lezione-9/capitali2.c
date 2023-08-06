#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // rischiesto per usare errno

typedef struct {
    char *nome;
    double lat;
    double lon;
} capitale;


void termina(const char *messaggio);

capitale *crea_capitale(char *nome, double lat, double lon){
    //controllo che il nome non sia nullo 
    //controllo che la latitudine e la longitudine siano nei range
    //-90 <= lat <= 90
    //-180 <= lon <= 180
    assert(nome != NULL);
    assert(lat>=-90 && lat<=90);
    assert(lon>=-180 && lon<=180);
    //creo lo spazio per l'oggetto
    capitale *a = malloc(sizeof(*a));
    a -> nome = strdup(nome);
    a -> lat = lat;
    a -> lon = lon;
    
    return a;
}

capitale *leggi_capitale(FILE *f){
    assert(f != NULL);
    char *nome;
    double lat, lon;
    //leggo dal file
    int e = fscanf(f, "%ms %lf %lf", &nome, &lat, &lon);
    if(e != 3){
        return NULL;
    }
    capitale *c = crea_capitale(nome, lat, lon);
    free(nome);
    return c;
}

capitale **leggi_capitale_da_file(FILE *f, int *n){
    assert(f != NULL);
    int size = 10;
    int messi = 0;

    //creo l'array di puntatori a capitale
    capitale **array = malloc(size*sizeof(capitale *));
    //controllo che la malloc sia andata a buon fine
    if(array == NULL) termina("Memoria insufficiente");

    while (true)
    {
        //leggo una capitale
        capitale *c = leggi_capitale(f);
        if(c == NULL) break;

        //inserisco la capitale nell'array, facendo tutti i controllli necessari 
        if(messi==size){
            size = size*2;
            array = realloc(array, size*sizeof(capitale *));
            if(array == NULL) termina("Realloc fallita");
        }
        assert(messi<size);
        array[messi] = c;
        messi += 1;
    }
    size = messi;
    array = realloc(array, size*sizeof(capitale *));
    if(array == NULL) termina("Realloc fallita");

    *n = messi;
    return array;

}

void stampa_capitale(capitale *c, FILE *f){
    fprintf(f,"%20s (%f,%f)\n", c->nome, (*c).lat, c->lon);
}

void capitale_distruggi(capitale *c){
    free(c->nome);
    free(c);
}




int main(int argc, char const *argv[])
{
    //controllo se è stato messo il nome del file
    if(argc != 2){
        printf ("USO: %s nomeFile\n", argv[0]);
        exit(1);
    }

    //mi prendo il nome del file da *argv[]
    FILE *f = fopen(argv[1], "r");
    
    //creo un array di puntatori di capitali leggendole dal file
    int n; // numero di capitali lette
    capitale **array = leggi_capitale_da_file(f, &n);

    //chiudo il file
    fclose(f);

    //stampo l'array
    for(int i=0; i<n; i++){
        stampa_capitale(array[i], stdout);
    }

    //distruggo l'array
    for(int i=0; i<n; i++){
        capitale_distruggi(array[i]);
    }
    free(array);
    return 0;
}







// stampa su stderr il  messaggio che gli passo
// se errno!=0 stampa anche il messaggio d'errore associato 
// a errno. dopo queste stampe termina il programma
void termina(const char *messaggio)
{
    if(errno==0) 
        fprintf(stderr,"%s\n",messaggio);
    else 
        perror(messaggio);
    exit(1);
}