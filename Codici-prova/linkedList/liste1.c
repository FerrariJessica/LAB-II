#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

struct node{
	int data;
	struct node *next;
}node;

void list_insert_head(struct node **head, int newdata){
	struct node *nuovo = (struct node *)malloc(sizeof(struct node));
	if (nuovo == NULL){
        printf("malloc fallita");
        exit(0);
    } 
	
	nuovo->data = newdata;
	nuovo->next = (*head);

	(*head) = nuovo;

}

void list_insert_tail(struct node *p, int newdata){
    struct node *nuovo = (struct node *)malloc(sizeof(struct node));
    if (nuovo == NULL){
        printf("Malloc fallita");
        exit (0);
    }

    while(p->next != NULL){
        p = p->next;
    }
    p->next = nuovo;
    nuovo->next = NULL;
    nuovo->data = newdata; 
}


void list_display(struct node *p){
	printf("\n");
	
	while (p != NULL){
		printf("|%d| -> ", p->data);
		p = p->next;
	}
	printf("NULL\n");
}

int list_search(struct node **head, int cercato){
	struct node *corrente = *head;

	while (corrente != NULL){
		if(corrente->data == cercato) return 1;
		corrente = corrente->next;
	} 
	return 0;
}

void list_delete(struct node **head, int data){
	struct node *tmp = *head;
	struct node *prev = NULL;
	//controllo se si sta eliminando la testa
	if(tmp != NULL && tmp->data == data){
		*head = tmp->next;
		free(tmp);
		return;
	}

	//scorro al nodo dopo
	while (tmp != NULL && tmp->data != data){
		prev = tmp;
		tmp = tmp->next;
	}	

	//non trovo il nodo
	if(tmp == NULL){
		printf("non esiste nessun nodo con data : %d, impossibile eliminare.\n", data);
		return;
	}
	
	//ho trovato il nodo
	prev->next = tmp->next;
	free(tmp);	

}

int main(int argc, char const *argv[])
{
    struct node *head;
    struct node *node1 = NULL;
    struct node *node2 = NULL;
    struct node *node3 = NULL;

    node1 = malloc(sizeof(struct node));
    node2 = malloc(sizeof(struct node));
    node3 = malloc(sizeof(struct node));

    node1->data = 12;
    node1->next = node2;

    node2->data = 22;
    node2->next = node3;

    node3->data = 32;
    node3->next = NULL;

    head = node1;

    list_display(head);
    int newdata = 45;
    list_insert_head(&head, newdata );
    list_display(head);

    int tosearch = 12;
    if (list_search(&head, tosearch) == 1){
        printf("il nodo %d è presente nella lista\n", tosearch);
    } else {
        printf("il nodo %d è NON presente nella lista\n", tosearch);
    }

    tosearch = 65;
    if (list_search(&head, tosearch) == 1){
        printf("il nodo %d è presente nella lista\n", tosearch);
    } else {
        printf("il nodo %d è NON presente nella lista\n", tosearch);
    }

    list_delete(&head, 45);
    list_display(head);
    list_delete(&head,22);
    list_display(head);
    list_delete(&head, 87);
    list_display(head);

    list_insert_tail(head, 54);
    list_display(head);

    return 0;
}
