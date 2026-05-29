#include <stdlib.h>
#include "pila.h"

void inicializarPila(Pila *p){
    p->cima = NULL;
}

int pilaVacia(Pila *p){
    return p->cima == NULL;
}

void push(Pila *p, void *dato){
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato = dato;
    nuevo->sig = p->cima;
    p->cima = nuevo;
}

void *pop(Pila *p){
    if(pilaVacia(p)) return NULL;

    Nodo *temp = p->cima;
    void *dato = temp->dato;

    p->cima = temp->sig;
    free(temp);

    return dato;
}

void *tope(Pila *p){
    if(pilaVacia(p)) return NULL;
    return p->cima->dato;
}
