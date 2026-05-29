#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include "Nodo.h"

typedef struct
{
    Nodo *inicio;
    Nodo *fin;
    int cantidad;
    void (*imprimir)(void*);
    int (*comparar)(void*, void*);
} Lista;

typedef struct
{
    Nodo *ante;
    Nodo *enco;
    void *dato;
} Resultado;

Resultado buscar(Lista lista, void *dato);

void eliminarInicio(Lista *lista);
void eliminarFinal(Lista *lista);
void eliminarDato(Lista *lista, void *dato);

void insertarinicio(Lista *lista, void *dato);
int insertaren(Lista *lista, void *dato, int index);
int eliminaren(Lista *lista, int index);

void imprimirResultado(Lista lista, Resultado resultado);
void agregarFinal(Lista *lista, void *dato);
void agregarEnOrden(Lista *lista, void *dato);
void imprimirLista(Lista lista);

#endif


