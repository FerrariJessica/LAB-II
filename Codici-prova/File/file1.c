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

    FILE *filehandle = fopen("codice.txt", "w");
    if (filehandle == NULL){
        printf("Errore nell'apertura del file");
    }

    return 0;
}
