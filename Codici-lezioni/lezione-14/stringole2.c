#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno

void termina(const char *messaggio);

typedef struct stringola{
    char *str;
    struct stringola *next;   
}stringola;

stringola *crea(char *s){
    assert (s != NULL);
    stringola *string = malloc(sizeof(*string));
    string -> str = strdup(s);
    string -> next = NULL;
    return string;
}

void distruggi(stringola *s){
    free(s->str);
    free(s);
}

void stampa(FILE *f, stringola *s){
    fprintf(f, "%20s\n", s->str);
}

void stampa_lst(FILE *f,stringola *s){
    while(s != NULL){
        stampa(f, s);
        s = s->next;
    }
}

void distruggi_lst(stringola *lis){

    if(lis!=NULL) {
        distruggi_lst(lis->next);
        distruggi(lis);
    }
}

// input :
// lis=lista di stringole può essere vuota ma deve essere ordinata lessicograficamente
// c=stringola da inserire deve esistere c!=NULL
// output la nuova lista con c inserita matenemdo l'ordine
stringola *inserisci_lex(stringola *lis, stringola *c) {
    assert(c!=NULL);
    
    //lista vuota
    if(lis == NULL){
        c->next = NULL;
        return c;
    }

    if(strcmp(c->str, lis->str)<0){
        c->next = lis;
        return c;
    } else{
        lis->next = inserisci_lex(lis->next, c);
        return lis;
    }
}

// "elimina" gli spazi in testa a una stringa
char *elimina_spazi_testa(char s[])
{
    int i=0;
    while(s[i]==' ')
        i++;
    assert(s[i]!=' ');
    return &s[i];
}



    
// main che legge le linee e le spezza al ;
int main(int argc, char const *argv[])
{
    if(argc!=2) {
        printf("Uso: %s nomefile\n",argv[0]);
        exit(1);
    } 

    FILE *f = fopen(argv[1], "r");
    if(f==NULL) termina("Errore nell'apertura del file");
    //costruzione della lista di stringhe leggendo dal file
    stringola *lis = NULL;
    //uso getline per leggere dal file e quindi mi serve un buffer dove contenere la parola
    char *buffer=NULL;
    size_t n = 0;
    while (true)
    {
        ssize_t e = getline(&buffer, &n, f);
        if(e<0){
            free(buffer);
            break;
        }

        if(buffer[e-1]=='\n') buffer[e-1] = '\0';   
        char *s = strtok(buffer, ";");
        while (s!=NULL)
        {
            s = elimina_spazi_testa(s);
            if(s[0]!='\0'){
                stringola *c = crea(s);
                lis = inserisci_lex(lis, c);
            }
            s = strtok(NULL, ";");
        }
        
        
    }
    
    fclose(f);
    stampa_lst(stdout, lis);
    distruggi_lst(lis);


    return 0;

}

void termina(const char *messaggio)
{
  if(errno==0) 
     fprintf(stderr,"%s\n",messaggio);
  else 
    perror(messaggio);
  exit(1);
}
