#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

//definisco un nuovo tipo di dato
typedef struct{
    char titolo[30];
    char nomeCanale[30];
    int views;
} videoYouTube;

int main (int argc, char *argv[] ){
    //creo un videoYouTube
    videoYouTube video1 = {.titolo= "Biscotto", .nomeCanale= "Cookies", .views= 1334};
    //stampo il video
    printf("il video %s realizzato da %s ha fatto %d views\n", video1.titolo, video1.nomeCanale, video1.views );

    //puntatore a struttura 
    videoYouTube *ptVideo = &video1;

    //posso cambiare il contenuto del video1 con il puntatore ptVideo

    (*ptVideo).views = 3000;
    printf("\nIl video %s ha ora %d vies\n", (*ptVideo).titolo, (*ptVideo).views);

   
    //puntatore freccia
    ptVideo -> views = 6000;
    printf("\nIl video %s ha ora %d vies\n", (*ptVideo).titolo, (*ptVideo).views);

    return 0;
}
