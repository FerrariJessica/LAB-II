#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *f = fopen("mascherine.txt", "a");

    if (f == NULL){
        printf("Errore nell'apertura del file\n");
        exit(1);
    }else printf("file aperto\n");

    /*printf("inserire il numero di mascherine totali: ");
    int mask;
    int e = scanf("%d", &mask);
    printf("Sono disponibili %d mascherine.\n\n", mask);
    if(e!=1){
        fprintf(stderr,"errore nella scanf mask");
    }*/

    while(1){
        char nome[30];
        int count = 0;

        printf("Inserire il nome : ");
        int e = scanf("%s", nome);
        if(e!=1){
            fprintf(stderr, "errore nella scanf del nome");
            exit(1);
        }
        printf("Inserire il numero di macherine che si vogliono comprare : ");
        e = scanf("%d", &count);
        if(e!=1){
            fprintf(stderr, "errore nella scanf del numero");
            exit(1);
        }
        printf("count : %d\n", count);
        
        if(count<=0) break;
        //printf("mask : %d\n", mask);
        //mask = mask - count;

        fprintf(f, "Nome: %s - Mascherine: %d\n", nome, count);
        //printf("sono rimaste %d mascherine\n", mask);
    }

    fclose(f);

    return 0;
}
