#ifndef PILA_H
#define PILA_H

typedef struct nodo{
    void *dato;
    struct nodo *sig;
}Nodo;

typedef struct{
    Nodo *cima;
}Pila;

/* operaciones de pila */

void inicializarPila(Pila *p);
int pilaVacia(Pila *p);
void push(Pila *p, void *dato);
void *pop(Pila *p);
void *tope(Pila *p);

#endif
