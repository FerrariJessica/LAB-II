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

    //Apertura fifo in scrittura
    int fd = open(argv[1], O_WRONLY);
    if(fd==-1){
        xtermina("Errore apertura FIFO", __LINE__, __FILE__);
    } 

    //Scrivo nello FIFO
    int scritti = 0;
    for(int i=0; i<x; i++){
        char c = 'a'+i;
        ssize_t e = write(fd, &c, sizeof(c));
        if(e!=sizeof(c)){
            xtermina("Errore scrittura FIFO", __LINE__, __FILE__);
        }
        scritti++;
    }

    //Chiudo fifo in scrittura
    close(fd);

    printf("[%d] : Sono stati scritti %d elementi\n",getpid(), scritti);


    return 0;
}