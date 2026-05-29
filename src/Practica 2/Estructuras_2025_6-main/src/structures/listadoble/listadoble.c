#include "listadoble.h"
#include <stdlib.h>

ListaD inicializarListaD(void)
{
    ListaD lista;
    lista.inicio = NULL;
    lista.fin = NULL;
    lista.cant = 0;
    lista.comparar = NULL;
    lista.imprimir = NULL;
    lista.liberar = NULL;
    return lista;
}

void mostrarListaD(ListaD lista)
{
    NodoD *aux = lista.inicio;

    while(aux)
    {
        lista.imprimir(aux->dato);
        aux = aux->sig;
    }
}

void insertarInicioD(ListaD *lista, void* dato)
{
    NodoD *nuevo = crearNodoD(dato);

    if(lista->inicio == NULL)
    {
        lista->inicio = lista->fin = nuevo;
    }
    else
    {
        nuevo->sig = lista->inicio;
        lista->inicio->ant = nuevo;
        lista->inicio = nuevo;
    }

    lista->cant++;
}

void insertarFinalD(ListaD *lista, void* dato)
{
    NodoD *nuevo = crearNodoD(dato);

    if(lista->fin == NULL)
    {
        lista->inicio = lista->fin = nuevo;
    }
    else
    {
        nuevo->ant = lista->fin;
        lista->fin->sig = nuevo;
        lista->fin = nuevo;
    }

    lista->cant++;
}

void insertarOrdenadoD(ListaD *lista, void* dato)
{
    if(lista->inicio == NULL)
    {
        insertarInicioD(lista,dato);
        return;
    }

    NodoD *aux = lista->inicio;

    while(aux && lista->comparar(aux->dato,dato) < 0)
    {
        aux = aux->sig;
    }

    if(aux == lista->inicio)
    {
        insertarInicioD(lista,dato);
    }
    else if(aux == NULL)
    {
        insertarFinalD(lista,dato);
    }
    else
    {
        NodoD *nuevo = crearNodoD(dato);

        nuevo->sig = aux;
        nuevo->ant = aux->ant;

        aux->ant->sig = nuevo;
        aux->ant = nuevo;

        lista->cant++;
    }
}

void* buscarDatoD(ListaD lista, void *dato, int (*comparar)(void*,void*))
{
    NodoD *aux = lista.inicio;

    while(aux)
    {
        if(comparar(aux->dato,dato) == 0)
            return aux->dato;

        aux = aux->sig;
    }

    return NULL;
}

void borrarDatoD(ListaD *lista, void* dato, int (*comparar)(void*,void*))
{
    NodoD *aux = lista->inicio;

    while(aux)
    {
        if(comparar(aux->dato,dato) == 0)
        {
            if(aux->ant)
                aux->ant->sig = aux->sig;
            else
                lista->inicio = aux->sig;

            if(aux->sig)
                aux->sig->ant = aux->ant;
            else
                lista->fin = aux->ant;

            lista->liberar(aux->dato);
            free(aux);
            lista->cant--;
            return;
        }

        aux = aux->sig;
    }
}

void borrarListaD(ListaD *lista)
{
    NodoD *aux = lista->inicio;

    while(aux)
    {
        NodoD *temp = aux;
        aux = aux->sig;

        lista->liberar(temp->dato);
        free(temp);
    }

    lista->inicio = NULL;
    lista->fin = NULL;
    lista->cant = 0;
}

void reordenarD(ListaD *lista, int (*comparar)(void*,void*))
{
    ListaD nueva = inicializarListaD();
    nueva.comparar = comparar;
    nueva.imprimir = lista->imprimir;
    nueva.liberar = lista->liberar;

    NodoD *aux = lista->inicio;

    while(aux)
    {
        insertarOrdenadoD(&nueva,aux->dato);
        aux = aux->sig;
    }

    lista->inicio = nueva.inicio;
    lista->fin = nueva.fin;
    lista->cant = nueva.cant;
    lista->comparar = nueva.comparar;
}
