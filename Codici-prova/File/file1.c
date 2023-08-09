#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int codice;
    printf("inserire un codice: ");
    int e = scanf("%d", &codice);
    if (e!=1){
        exit(1);
    }

    //Scrittura del file
    FILE *filehandle = fopen("codice.txt", "w");
    if (filehandle == NULL){
        printf("Errore nell'apertura del file\n");
        exit(1);
    }else printf("file aperto\n");

    fprintf(filehandle, "%d", codice);
    printf("ho scritto il codice %d nel file", codice);

    fclose(filehandle);
    printf("file chiuso\n");

    //Lettura del file
    int letto;
    filehandle = fopen("codice.txt", "r");
    if (filehandle == NULL){
        printf("Errore nell'apertura del file\n");
        exit(1);
    }else printf("file aperto\n");

    e = fscanf(filehandle, "%d", &letto);
    if (e!=1){
        printf("Errore lettura del file");
        exit(1);
    }
    printf("ho letto il codice %d\n" , letto );
    fclose(filehandle);
    printf("file chiuso\n");


    return 0;
}
