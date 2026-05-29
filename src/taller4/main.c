#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void imprimirInt(void *a) {
    if (a) printf("%d", *(int*)a);
}

int compararInt(void *a, void *b) {
    return (*(int*)a - *(int*)b);
}

void imprimirRama(void **datos, int n, int i, int nivel, char *prefijo, int banderas[]) {
    if (i >= n) return; //Llego al final de la rama


//Si la iteracion es mayor o igual a n. Iteracion hasta que llegue
//al valor de nivel, si j es igual a nivel-1 se imprime prefijo
    for (int  j=0;j<nivel; j++) {
        if (j==nivel-1) printf("|----%s ", prefijo);
        else printf(banderas[j] ? "|    " : "     ");
    }

    printf("%d\n", *(int*)datos[i]);
    //Hijo Izquierdo: Siempre está en 2 *i+1.
    //Hijo Der: Siempre está en 2 * i+2

    int izq =2*i+1,der=2*i+2;

    if (izq<n || der < n) {
        for (int j = 0; j <= nivel; j++) {
            printf((banderas[j] || (j == nivel && izq < n && der < n)) ? "|    " : "     ");
        }
        printf("\n");
    }

    banderas[nivel] = (izq < n);
    if (der < n) imprimirRama(datos, n, der, nivel + 1, "R", banderas);

    banderas[nivel] = 0;
    if (izq<n) {
        if (der < n) printf("|\n");
        imprimirRama(datos, n, izq, nivel + 1, "I", banderas);
    }
}

void visualizarHeap(Heap h) {
    int banderas[100] = {0};
    printf("Imprimir el Arbol\n");
    if (h.cantidad > 0) imprimirRama(h.datos, h.cantidad, 0, 0, "", banderas);
}

int main() {
    Heap h = crearHeap(HEAP_MAX, compararInt, imprimirInt);
    int valores[] = {11,6,8,5,3,2,1};
    int total = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < total; i++) {
        int *v = malloc(sizeof(int));
        *v = valores[i];
        insertarHeap(&h, v);
    }

    visualizarHeap(h);

    for (int i = 0; i < h.cantidad; i++) free(h.datos[i]);
    return 0;
}
