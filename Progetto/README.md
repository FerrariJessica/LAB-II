# ProgettoArchivio
Progetto finale del corso di Laboratorio 2

## Organizzazione file

- archivio.c
    In questo file è presente il programma che viene fatto partire dal server. Riceve in input il numero di thread scrittori e di thread lettori da creare e restituisce quanti lettori e quanti scrittori sono stati creati.
     ###Thread capo scrittore
     Questo thread legge delle sequenze di byte dalla named pipe caposc. Legge per prima la dimensione di ogni sequenza e poi la sequenza stessa.
     Dopo aver letto ogni sequenza la tokenizza con strtok, inserisce una copia del token nel buffer produttore consumatore gestito con i semafori.


## COMPILAZIONE : 
Usare il comando make
## ESECUZIONE : 
./archivio.out #threadScrittori #threadLettori
./provaScrittura.out fileprova

-> Se non è presente creare la fifo "caposc" dal terminale in quanto la dovrà creare il server. 

## Problemi 
 - se eseguo ./archivio.out 3 3 e ./provaScrittura.out fileprova4 va in segmentation fault (ci va anche se aggiungo una stringa nel file)
 - se eseguo ./archivio.out 3 3 e ./provaScrittura.out fileprova6 va legge un carattere che non deve strano(lo fa anche se aggiungo una stringa nel file)
 - se eseguo ./archivio.out 3 3 e ./provaScrittura.out fileprova1 (o 2 o 3 o altri file) è corretto 
