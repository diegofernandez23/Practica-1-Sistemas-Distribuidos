#include "claves.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR 255
#define MAX_V2  32

typedef struct Nodo {
    //key, value1, N_value2, V_value2, value3
    //El +1 es para el caracter nulo de fin en los string
    char key[MAX_STR + 1];
    char value1[MAX_STR + 1];
    int N_value2;
    float V_value2[MAX_V2];
    struct Paquete value3;
    //Puntero al siguiente nodo de la lista enlazada
    struct Nodo *next;
} Nodo;

//Puntero al inicio de la lista enlazada
static Nodo *head = NULL;

//Funcion auxiliar para encontrar un nodo por la clave
static Nodo* find_node(const char *key) {
    //Se posicona el puntero al inicio de la lista enlazada
    Nodo *p = head;
    //La recorre hasta encontrar un nodo con la clave
    //O hasta llegar al final de la lista (p == NULL)
    while (p != NULL){
        if (strcmp(p->key, key) == 0)
            return p;
        p= p->next;
    }
    //Si no se encuentra el nodo se devuelve NULL
    return NULL;
}

int destroy(void) {

    //Se posiciona el puntero al inicio de la lista enlazada
    Nodo *p = head;
    //Se recorre la lista enlazada liberando cada nodo
    while (p != NULL) {
        Nodo *next = p->next;
        free(p);
        p = next;
    }
    //Se establece el puntero head a NULL
    head=NULL;
    return 0;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

    //Validacion si los punteros son nulos
    if (key==NULL || value1 == NULL || V_value2 == NULL)
        return -1;

    //Validacion de longitud de los strings
    if (strlen(key) > MAX_STR || strlen(value1) > MAX_STR)
        return -1;

    //Validacion del rango de N_value2
    if (N_value2 < 1 || N_value2 > MAX_V2)
        return -1;

    //Validacion de existencia de la clave
    if (find_node(key) != NULL)
        return -1;

    //Creacion del nuevo nodo
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    //Validacion de la asignacion de memoria
    if (nuevo == NULL)
        return -1;

    //Se copian los datos al nuevo nodo
    strcpy(nuevo->key, key);
    strcpy(nuevo->value1, value1);
    nuevo->N_value2 = N_value2;

    for (int i=0; i < N_value2; i++)
        nuevo->V_value2[i] = V_value2[i];

    nuevo->value3 = value3;

    //Se inserta el nuevo nodo al inicio de la lista enlazada
    nuevo->next = head;
    //Se actualiza el puntero head para que apunte al nuevo nodo
    head = nuevo;

    return 0;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
        
    //Validacion de punteros nulos y longitud
    if (key == NULL || value1 == NULL || N_value2 == NULL || V_value2 == NULL || value3 == NULL || strlen(key) > 255)
        return -1;

    //Encontrar el nodo correspondiente a la clave
    Nodo *n = find_node(key);
    //Si no se encuentra el nodo, se devuelve -1
    if (n==NULL)
        return -1;

    //Se copian los datos del nodo encontrado a los punteros de salida
    strcpy(value1, n->value1);
    *N_value2 = n->N_value2;
    for (int i=0; i < n->N_value2; i++)
        V_value2[i] = n->V_value2[i];
    *value3 = n->value3;

    return 0;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {

    //Comprobacion de punteros nulos
    if (key==NULL || value1==NULL || V_value2==NULL)
        return -1;

    //Validacion de longitud de los strings
    if (strlen(key)>MAX_STR || strlen(value1) > MAX_STR)
        return -1;

    //Validacion del rango de N_value2
    if (N_value2 < 1 || N_value2 > MAX_V2)
        return -1;

    //Encontrar el nodo correspondiente a la clave
    Nodo *n = find_node(key);
    if (n==NULL)
        return -1;

    //Se copian los nuevos datos al nodo encontrado
    strcpy(n->value1, value1);
    n->N_value2 = N_value2;

    //Se ponen todos a 0.0f para limpiar datos anteriores
    for (int i = 0; i < MAX_V2; i++)
        n->V_value2[i] = 0.0f;

    //Se copian los nuevos valores al nodo
    for (int i = 0; i < N_value2; i++)
        n->V_value2[i] = V_value2[i];

    n->value3 = value3;

    return 0;
}

int delete_key(char *key) {

    //Validacion de puntero nulo
    if (key == NULL || strlen(key) > 255)
        return -1;

    //Nodo anterior
    Nodo *prev = NULL;
    //Nodo actual
    Nodo *curr = head;

    //Se recorre la lista enlazada buscando el nodo
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {

            //Si es el primer nodo, head pasa al siguiente
            if (prev == NULL)
                head = curr->next;
            //Si no, el nodo anterior apunta al siguiente del nodo actual
            else
                prev->next=curr->next;

            //Se libera la memoria del nodo encontrado
            free(curr);
            return 0;
        }
        //El actual pasa a ser el anterior y el siguiente pasa a ser el actual
        prev = curr;
        curr = curr->next;
    }

    //Si no se encuentra el nodo se devuelve -1
    return -1;
}

int exist(char *key) {

    //Validacion de puntero nulo
    if (key == NULL || strlen(key) > 255)
        return -1;
    
    if (find_node(key) != NULL)
        return 1;
    else
        return 0;
}