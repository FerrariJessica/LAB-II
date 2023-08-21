#include "xerrori.h"

int main(int argc, char const *argv[])
{
  //creo un numero di processi figli indicato dall'utente
  if(argc != 2){
    fprintf(stderr, "Uso : %s n\n", argv[0]);
    exit(1);
  }
  int nfigli = atoi(argv[1]);
  //creo i processi figli
  for(int i=0; i<nfigli; i++){
    pid_t pid = xfork(__LINE__,__FILE__);
    if(pid == 0){
      //figlio
      //deve aprire la pipe somma_down in lettura e 
      //leggere da li interi a 32 bit
      
    }
  }


  return 0;
}
