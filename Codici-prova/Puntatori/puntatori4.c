#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[]){
    int x = 69;
    int *p1 = &x;
    int **p2 = &p1;
    int ***p3 = &p2;

    printf("Valore di x: %d - Indirizzo di x: %p\n", x, &x);
    printf("Valore di p1: %p - Indirizzo di p1: %p\n", p1, &p1);
    printf("Valore di p2: %p - Indirizzo di p2: %p\n", p2, &p2);
    printf("Valore di p3: %p - Indirizzo di p3: %p\n", p3, &p3);

    printf("\nStampa di x dai puntatori:\n");
    printf("Valore di x da p1 %d\n", *p1);
    printf("Valore di x da p2 %d\n", **p2);
    printf("Valore di x da p3 %d\n", ***p3);

    //se si usa l'array
    int a[3] = {1, 2, 3};
    int *pt1 = a;
    int **pt2 = &pt1;
    int ***pt3 = &pt2;

    printf("\nIndirizzo di a: %p\n", a);
    //stampo l'indirizzo di a mediante pt3
    printf("Indirizzo di a mediante p3: %p\n", **pt3);
    //stampo l'elemento a[1]
    printf("a[1] mediante p3: %d\n", (**pt3)[1]);
    //stampo l'elemento a[2] con l'aritmentica dei puntatori
    printf("a[2] mediante p3 e aritmetica dei puntatori: %d\n", *(**pt3+2));
    return 0;
}