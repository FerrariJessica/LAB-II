#include "xerrori.h"

int main(int argc, char *argv[])
{
    if(argc!=3){
        printf("Uso:\n\t%s nome_pipe intero\n",argv[0]);
        exit(1);  
    }
    int x = atoi(argv[2]);

    // Creazione FIFO
    int e = mkfifo(argv[1], 0666);
    if(e==0){
        printf("FIFO creato\n");
    }else {
        if(errno==EEXIST){
            printf("FIFO già esistente\n");
        } else {
            xtermina("Errore creazione FIFO", __LINE__, __FILE__);
        }
    }

    //Apertura fifo in lettura
    int fd = open(argv[1], O_RDONLY);
    if(fd==-1){
        xtermina("Errore apertura FIFO", __LINE__, __FILE__);
    }
    printf("Aperta FIFO: %d\n", fd);

    //Leggo dalla fifo
    char c;
    for (int i=0; i<x; i++){
                ssize_t e = read(fd, &c, sizeof(c));
        if(e!=sizeof(c)){
            xtermina("Errore lettura FIFO", __LINE__, __FILE__);
        }
        printf("Lettura FIFO: %c\n", c);
    }

    //Chiudo fifo in lettura
    close(fd);

}    