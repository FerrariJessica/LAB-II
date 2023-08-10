#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa/numero rand() abs() exit()
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <string.h>   // prototipi delle funzioni per stringhe

//stampa un messaggio di errore e termina
void termina(const char *messaggio){
    perror(messaggio);
    exit(EXIT_FAILURE);
}

void stampa_matrice_statica(int r, int c, int m[][c], FILE *f){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            fprintf(f, "righa %d colonna %d :%2d   ", i, j, m[i][j]);
        }
        fprintf(f, "\n");
    }
}

int **crea_matrice_din(int r, int c){
    int **rig;
    //prima alloco tutte le righe e poi dentro ad ogni righa alloco le colonne
    rig = malloc(r*sizeof(*rig));
    for(int i=0; i<r; i++){
        rig[i]=malloc(c*sizeof(int));
        if(rig[i]==NULL) termina("Allocazione colonna fallita");
    }

    return rig;
}

void stampa_matrice_dinamica(int r, int c, int **m, FILE *f){
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            fprintf(f, "righa %d colonna %d :%2d   ", i, j, m[i][j]);
        }
        fprintf(f, "\n");
    }
}


int main(int argc, char const *argv[])
{
    if(argc != 3){
        printf("Uso: %s n.righe n.colonne\n", argv[0]);
        exit(1);
    }
    
    //allocazione statica di una matrice
    int righe = atoi(argv[1]);
    int colonne = atoi(argv[2]);
    if(righe<=0 || colonne <=0){
        termina("le righe e le colonne devono essere interi positivi\n");
    }
    int matrice [righe][colonne];

    for(int i=0; i<righe; i++){
        for(int j=0; j<colonne; j++){
            matrice[i][j] = i+j;
        }
    }
    puts("stampa matrice statica \n");
    stampa_matrice_statica(righe, colonne, matrice, stdout);


    //allocazione dinamica di una matrice
    int **matrdin;
    matrdin = crea_matrice_din(righe, colonne);

    for(int i=0; i<righe; i++){
        for(int j=0; j<colonne; j++){
            matrdin[i][j] = i+j;
        }
    }


    puts("\nstampa matrice dinamica\n");
    stampa_matrice_dinamica(righe, colonne, matrdin, stdout);

    return 0;
}
