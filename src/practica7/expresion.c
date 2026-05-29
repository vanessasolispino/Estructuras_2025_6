#include <math.h>
#include "expresion.h"

double evaluar(Arbol arbol)
{
	NodoA **arrOrigin = calloc(arbol.cantidad, sizeof(NodoA*));
	if(!arrOrigin)
	{
		printf("\n NO SE PUDO ASIGNAR EL ARREGLO DE NODOS");
		exit(1);
	}
	int cantidad = 0;
	extraerNodos(arbol.raiz, arrOrigin, &cantidad);
	asignarMemoria(arrOrigin, cantidad, arbol.comparar);
	capturarValores(arrOrigin, cantidad);

	arbol.imprimir = imprimirValor;
	printf("\n ARBOL VALORES\n");
	imprimirArbol(arbol);

    double resultadoFinal = calcularPostOrden(arbol.raiz);
	printf("\n EL RESULTADO DE LA EXPRESION ES: %.2lf  \n", resultadoFinal);

	liberarMemoriaValores(arrOrigin, cantidad);
	free(arrOrigin);

	return resultadoFinal; // Retornar el resultado real
}


void extraerNodos(NodoA *raiz, NodoA **nodos, int *i) {
	if(!raiz) return;
	extraerNodos(raiz->izq, nodos, i);
	nodos[(*i)] = raiz;
	(*i)++;
	extraerNodos(raiz->dch, nodos, i);
}

void asignarMemoria(NodoA **original, int cantidad, int (*comparar)(void*, void*)) {
	int *indices = (int*)calloc(cantidad, sizeof(int));
	for(int i = 0; i < cantidad; i++) indices[i] = -1;
	for(int i = 0 ; i < cantidad; i++) {
		if(indices[i] != -1) continue;
		if(precedencia(original[i]->dato) != OPERANDO) continue;

		for(int j = i; j < cantidad; j++) {
			if(comparar(original[i]->dato, original[j]->dato) == 0)
				indices[j] = i;
		}
		double **ptr = calloc(1, sizeof(double*));
		for(int j = 0; j < cantidad; j++) {
			if(indices[j] == i)
				((Variable*)(original[j]->dato))->v = ptr;
		}
	}
	free(indices);
}

void capturarValores(NodoA **original, int cantidad) {
	for(int i = 0; i < cantidad; i++) {
		if(precedencia(original[i]->dato) != OPERANDO) continue;

		if(((Variable*)original[i]->dato)->v != NULL && *((Variable*)original[i]->dato)->v == NULL) {
			*((Variable*)original[i]->dato)->v = calloc(1, sizeof(double));
			printf("\n Captura variable %c: ", ((Variable*)original[i]->dato)->c);
			inputDouble("", *((Variable*)original[i]->dato)->v);
		}
	}
}

void liberarMemoriaValores(NodoA **original, int cantidad) {
	int *indices = (int*)calloc(cantidad, sizeof(int));
	for(int i = 0; i < cantidad; i++) indices[i] = -1;
	for(int i = 0; i < cantidad; i++) {
		if(precedencia(original[i]->dato) != OPERANDO) continue;
		if(((Variable*)original[i]->dato)->v != NULL) {
			for(int j = i; j < cantidad; j++) {
				if(((Variable*)original[i]->dato)->v == ((Variable*)original[j]->dato)->v)
					indices[j] = i;
			}
			free(*((Variable*)original[i]->dato)->v);
			free(((Variable*)original[i]->dato)->v);
			for(int j = 0; j < cantidad; j++) {
				if(indices[j] == i)
					((Variable*)original[j]->dato)->v = NULL;
			}
		}
	}
	free(indices);
}

double calcularPostOrden(NodoA* nodo) {
    if (nodo == NULL) return 0;
    if (nodo->izq == NULL && nodo->dch == NULL) {
        Variable* var = (Variable*)nodo->dato;
        if (var->v != NULL && *(var->v) != NULL) {
            return **(var->v);
        }
        return 0;
    }
    double izqVal = calcularPostOrden(nodo->izq);
    double dchVal = calcularPostOrden(nodo->dch);
    char operador = ((Variable*)nodo->dato)->c;
    switch (operador) {
        case '+': return izqVal + dchVal;
        case '-': return izqVal - dchVal;
        case '*': return izqVal * dchVal;
        case '/': return (dchVal != 0) ? izqVal / dchVal : 0;
        case '^': return pow(izqVal, dchVal);
    }
    return 0;
}
