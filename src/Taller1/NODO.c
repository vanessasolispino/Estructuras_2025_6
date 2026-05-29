//NODO.c
#include "Nodo.h"

Nodo* crearNodo(void *dato)
{
	Nodo *nuevo = (Nodo*)calloc(1,sizeof(Nodo));
	nuevo->dato = dato;
	return nuevo;	
}