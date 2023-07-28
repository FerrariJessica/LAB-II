#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct{
    char nome[30];
    int viewsTotali;
    float guadagnoStimato;
}canale;

int main(int argc, char *argv[]){
    
    canale *ptCanale;
    int n;

    printf("Inserire il numero di quanti canali si vogliono memorizzare : \n");
    int e = scanf("%d", &n);
    if(e!=1){
        printf("Errore nella scanf");
        exit(1);
    }

    //alloco la memoria per n strutture di tipo canale
    ptCanale = (canale*) malloc(n*sizeof(canale));

    //riempo l'array di canali
    for(int i=0; i<n; i++){
        printf("Nome canale: \n");
        e = scanf("%s", (ptCanale+i)->nome );
        if(e!=1){
            printf("Errore nella scanf");
            exit(1);
        }

        printf("Visualizzazioni: \n");
        e = scanf("%d", &(ptCanale+i)->viewsTotali );
        if(e!=1){
            printf("Errore nella scanf");
            exit(1);
        }

        //calcolo guadagno stimato
        (ptCanale + i) -> guadagnoStimato = (ptCanale + i)->viewsTotali/1000;
    }


    //stampo l'array dei canali
    printf("\nLista dei canali registrati:");
    for (int i=0; i<n; i++){
        printf(" \nNome: %s\nViews: %d \nGuadagno: %.2f\n", (ptCanale+i)->nome, (ptCanale+i)->viewsTotali, (ptCanale+i)->guadagnoStimato);
    }

    return 0;
}