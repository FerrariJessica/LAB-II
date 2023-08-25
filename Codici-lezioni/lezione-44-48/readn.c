#define _GNU_SOURCE   // permette di usare estensioni GNU
#include <stdio.h>    // permette di usare scanf printf etc ...
#include <stdlib.h>   // conversioni stringa exit() etc ...
#include <stdbool.h>  // gestisce tipo bool
#include <assert.h>   // permette di usare la funzione ass
#include <string.h>   // funzioni per stringhe
#include <errno.h>    // richiesto per usare errno
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*Fa read successive fino a che non riceve tutti i byte richiesti
    Parametri:
    - file descriptor
    - puntatore al buffer (di qualsiasi tipo)
    - numero di byte (size_t è un intero, di tipo unsigned long, con cui si rappresenta quanto è grande un file)

    Ritorno:
    - numero di byte letti di tipo ssize_t (ssize_t è un intero, di tipo signed long (con segno) , con cui si rappresenta quanto è grande un file)
*/

ssize_t readn(int fd, void *ptr, size_t n){
    
    size_t   nleft; // numero di byte da leggere
    ssize_t  nread; // numero di byte letti ogni read

    nleft = n; // inizialmente il numero di byte da leggere è n
    while (nleft > 0) {
        if((nread = read(fd, ptr, nleft)) < 0){
            if (nleft == n) return -1; /* error, return -1 */
            else break; /* error, return amount read so far */
        } else if (nread == 0) break; /* EOF */
        nleft -= nread;
        ptr   += nread;
    }
    return(n - nleft); /* return >= 0 */
}

/* Fa write successive fino a che non scrive tutti i byte richiesti
    Parametri:
    - file descriptor
    - puntatore al buffer (di qualsiasi tipo)
    - numero di byte (size_t è un intero, di tipo unsigned long, con cui si rappresenta quanto è grande un file)

    Ritorno:
    - numero di byte scritti di tipo ssize_t (ssize_t è un intero, di tipo signed long (con segno) , con cui si rappresenta quanto è grande un file)
*/

ssize_t writen(int fd, void *ptr, size_t n) {  
    size_t   nleft; // numero di byte da scrivere
    ssize_t  nwritten; // numero di byte scritti ogni write
  
    nleft = n; // inizialmente il numero di byte da scrivere è n
    while (nleft > 0) {
        if((nwritten = write(fd, ptr, nleft)) < 0) {
            if (nleft == n) return -1; /* error, return -1 */
            else break; /* error, return amount written so far */
        } else if (nwritten == 0) break; 
        nleft -= nwritten;
        ptr   += nwritten;
    }
    return(n - nleft); /* return >= 0 */
}