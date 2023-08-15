#define _GNU_SOURCE   // avverte che usiamo le estensioni GNU 
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero exit() etc ...
#include <stdbool.h>  // gestisce tipo bool (variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // confronto/copia/etc di stringhe
#include <errno.h>

// stampa un messaggio d'errore e termina il programma
void termina(const char *messaggio);


typedef struct sq{
    char *nome;
    int punti;
    struct sq *next;
} squadra;


squadra *crea_squadra(char *n, int p){
    squadra *nuova = malloc(sizeof(*nuova));
    nuova->nome = strdup(n);
    nuova->punti = p;
    nuova->next = NULL;
    return nuova;
}

int squadra_cmp_nome(squadra *a, squadra *b){
    // Se a è prima di b -1, se dopo 1, se uguali 0
    return strcmp(a->nome,b->nome);
}

squadra *inserisci_lex(squadra *lis, squadra *nuova, int(*cmp)(squadra *, squadra *)){
    assert(nuova != NULL);
    if(lis == NULL){
        nuova->next = NULL;
        return nuova;
    }
    if(cmp(nuova, lis) < 0){
        nuova->next = lis;
        return nuova;
    }
    lis->next = inserisci_lex(lis->next, nuova, cmp);
    return lis;
}

int squadra_cmp_punteggio(squadra *a, squadra *b){
    if(a->punti > b->punti) return -1;
    else if(a->punti < b->punti) return 1;
    else return 0;
}

squadra *ordina_punteggio(squadra *lis1, squadra *lis2){
    while(lis1 != NULL){
        squadra *nuova = crea_squadra(lis1->nome, lis1->punti);
        lis2 = inserisci_lex(lis2, nuova, squadra_cmp_punteggio);
        lis1 = lis1->next;
    }
    return lis2;
}


bool cerca_squadra(squadra *lis, char *n, squadra **s){
    while (lis != NULL)
    {
        printf("lis->nome: %p n: %p\n", lis->nome, n);
        if(strcmp(lis->nome, n) == 0){
            *s = lis;
            //printf("[LOG] cerca_squadra: %s\n", lis->nome);
            return true;
        }
        lis = lis->next;
    }
    return false;

}



squadra *aggiungi_punti(squadra *lis, char *n, int p){
    squadra *s = NULL;
    if(cerca_squadra(lis, n, &s)){
        //aggiungere i punti alla squadra nel puntatore s
        
        s->punti = s->punti + p;
        //printf("[LOG] aggiungi_punti: %s\n", s->nome);
        //free(s);
        return lis;
    } else {
        //creo la squadra e la inserisco nella lista
        squadra *nuova = crea_squadra(n, p);
        lis = inserisci_lex(lis, nuova, squadra_cmp_nome);
        return lis;
    }


}


squadra *aggiungi_partita(squadra *lis, char *s1, int g1, char *s2, int g2){
    lis = aggiungi_punti(lis, s1, g1);
    lis = aggiungi_punti(lis, s2, g2);
    return lis;
}




void stampa_squadra(squadra *s, FILE *f){
    fprintf(f, "%-20s: %d\n", s->nome, s->punti);
}

void scrivi_lista(squadra *lis, FILE *f){
    while (lis != NULL)
    {
        stampa_squadra(lis, f);
        lis = lis->next;
    }
    
}


void stampa_per_punti (squadra *lis, FILE *f){
    assert (lis != NULL);
    int p = lis->punti;
    while(lis != NULL){
        //scrivi_lista(lis, stdout);
        //printf("punti: %d || p: %d\n", lis->punti, p);
        fprintf(f, "%d ", p);
        // printf("lis: %p || lis->nome %p", lis, lis->nome);
        while(lis != NULL && lis->punti == p){
            fprintf(f, "%s ", lis->nome);
            lis = lis->next;
        }
        p--;
        fprintf(f, "\n");
    }

}



void squadra_distruggi(squadra *s){
    free(s->nome);
    free(s);
}

void distruggi_lista(squadra *lis){
    while (lis!=NULL)
    {
        squadra *tmp = lis->next;
        squadra_distruggi(lis);
        lis = tmp;
    }
}




int main(int argc, char const *argv[])
{
    puts("----Prova punto 1----");
    squadra *classifica = NULL;
    classifica = aggiungi_punti(classifica,"Juve",1);   
    // printf("%p", classifica);
    classifica = aggiungi_punti(classifica,"Chievo",3);   
    classifica = aggiungi_punti(classifica,"Milan",1);
    classifica = aggiungi_punti(classifica,"Juve",3);
    classifica = aggiungi_punti(classifica,"Spal",1);      // SEGSEV here
    classifica = aggiungi_punti(classifica,"Milan",1);
    classifica = aggiungi_punti(classifica,"Roma",3);
    scrivi_lista(classifica,stdout); // da scrivere in analogia a quanto fatto per le citta'
    distruggi_lista(classifica);     // da scrivere in analogia a quanto fatto per le citta'

    puts("\n----Punto 2----");
    if(argc!=2){
        printf("USO: %s nomefile\n", argv[0]);
        exit(0);
    }

    squadra *classifica2 = NULL;

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) termina("Errore nell'apertura del file");
    int g1, g2;
    char *s1 = NULL;
    char *s2 = NULL;
    
    while(true){
        int e = fscanf(f, "%d %d %ms %ms", &g1, &g2, &s1, &s2);
        //printf("e : %d, g1 : %d, g2 : %d, s1 : %s, s2 : %s\n", e, g1, g2, s1, s2);
        if (e!=4) break;
        classifica2 = aggiungi_partita(classifica2, s1, g1, s2, g2);
        //printf("%p", classifica2);
    }
    free(s1);
    free(s2);
    fclose(f);
    scrivi_lista(classifica2, stdout);

    puts("\n----Punto 3----");
    
    printf("lista ordinata per punteggio decrescente: \n");
    squadra *classificaP = NULL;
    classificaP = ordina_punteggio(classifica2, classificaP);

    scrivi_lista(classificaP, stdout);


    puts("\n----Punto 4----");
    stampa_per_punti(classificaP, stdout);

    distruggi_lista(classifica2);
    distruggi_lista(classificaP);

    return 0;
}





void termina(const char *messaggio){
  if(errno!=0) perror(messaggio);
	else fprintf(stderr,"%s\n", messaggio);
  exit(1);
}