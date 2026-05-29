#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostring.h>
#include <heap.h>

typedef struct {
    char nombre[50];
    int paginas;
} Archivo;

void* eliminarPosicion(Heap *h, int idx);
int compararArchivo(void* a, void* b);
void imprimirArchivo(void* a);
void mostrarEstadoCompleto(Heap h);
void imprimirComoArbol(Heap h, int idx, int nivel);
Archivo* generarArchivoAleatorio();

int main() {
    srand(time(NULL));
    Heap cola = crearHeap(HEAP_MIN, compararArchivo, imprimirArchivo);
    int opcion;

    do {
        printf("\nPRIORIDAD: %s", cola.tipo == HEAP_MIN ? "MENOR paginas (MIN)" : "MAYOR paginas (MAX)");

        printf("\nTOP 5 EN COLA:");
        for (int i = 0; i < (cola.cantidad < 5 ? cola.cantidad : 5); i++) {
            printf("\n  %d. ", i + 1);
            imprimirArchivo(cola.datos[i]);
        }
        printf("\n----------------------------------------");
        printf("\n[0] CAMBIAR PRIORIDAD (Invertir)");
        printf("\n[1] MOSTRAR COLA (Arreglo y Arbol)");
        printf("\n[2] AGREGAR ARCHIVO (Aleatorio)");
        printf("\n[3] PROCESAR/IMPRIMIR ARCHIVO (Extraer raiz)");
        printf("\n[4] ELIMINAR ARCHIVO ESPECIFICO (Por indice)");
        printf("\n[5] ELIMINAR TODOS LOS ARCHIVOS (Vaciar)");
        printf("\n[6] TERMINAR PROGRAMA");
        printf("\nSeleccione: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n'); // Limpiar buffer
            continue;
        }

        switch (opcion) {
            case 0:
                cola.tipo = (cola.tipo == HEAP_MIN) ? HEAP_MAX : HEAP_MIN;
                heapify(&cola);
                printf("\nPrioridad cambiada y cola reordenada.");
                mostrarEstadoCompleto(cola);
                break;

            case 1:
                mostrarEstadoCompleto(cola);
                break;

            case 2: {
                Archivo *nuevo = generarArchivoAleatorio();
                printf("\nAntes de instertar");
                mostrarEstadoCompleto(cola);

                insertarHeap(&cola, nuevo);

                printf("\nDespues");
                printf("\nAgregado: %s (%d pags)\n", nuevo->nombre, nuevo->paginas);
                mostrarEstadoCompleto(cola);
                break;
            }

            case 3:
                if (cola.cantidad > 0) {
                    printf("\nAntes de procesar");
                    mostrarEstadoCompleto(cola);

                    Archivo *doc = (Archivo*)eliminarHeap(&cola);
                    printf("\n imprimiendo: %s", doc->nombre);
                    free(doc);

                    printf("\nDespues de procesar");
                    mostrarEstadoCompleto(cola);
                } else printf("\nCola vacia.");
                break;

            case 4:
                if (cola.cantidad > 0) {
                    mostrarEstadoCompleto(cola);
                    int idx;
                    printf("\nIndice a eliminar (0-%d): ", cola.cantidad - 1);
                    scanf("%d", &idx);

                    Archivo *eliminado = (Archivo*)eliminarPosicion(&cola, idx);
                    if (eliminado) {
                        printf("\nEliminado %s", eliminado->nombre);
                        free(eliminado);
                    } else printf("\no valido");

                    mostrarEstadoCompleto(cola);
                }
                break;

            case 5:
                while (cola.cantidad > 0) free(eliminarHeap(&cola));
                printf("\nCola vaciada.");
                mostrarEstadoCompleto(cola);
                break;
        }
    } while (opcion != 6);

    // Liberar memoria final
    while (cola.cantidad > 0) free(eliminarHeap(&cola));
    free(cola.datos);

    return 0;
}


int compararArchivo(void *a, void *b) {
    Archivo *A = (Archivo*)a;
    Archivo *B = (Archivo*)b; //cast
    return A->paginas - B->paginas;//prioridad
}

void imprimirArchivo(void *a) {
    if(!a) return;
    Archivo *A = (Archivo*)a;
    printf("%s [%d paginas]", A->nombre, A->paginas);
}

void* eliminarPosicion(Heap *h, int idx) {
    if (idx < 0 || idx >= h->cantidad) return NULL; //validacion

    void *removido = h->datos[idx];
    h->datos[idx] = h->datos[h->cantidad - 1];
    h->cantidad--;
    heapify(h);

    return removido;
}

//notas para mi
/*Accedemos al heap h, arr interno datos y tomamos el elemento
 en la posición idx, guardándolo en removido.

Accedemos al heap h, luego a su arreglo interno
datos, y en la posición idx colocamos el último
elemento del heap (posición cantidad - 1), reemplazando el elemento eliminado.

Accedemos al heap h y reducimos en 1
la cantidad de elementos, indicando que el último ya no forma parte del heap.

Accedemos al heap h y
reordenamos su arreglo interno datos para restaurar la propiedad del heap.

Devolvemos el elemento que originalmente estaba en la posición idx.*/

Archivo* generarArchivoAleatorio() {
    Archivo *nuevo = malloc(sizeof(Archivo));
    sprintf(nuevo->nombre, "Archivo_%d.pdf", rand() % 100);
    nuevo->paginas = (rand() % 50) + 1;
    return nuevo;
}

// Arreglloyarbol
void mostrarEstadoCompleto(Heap h) {
    printf("\nRAIZ EN POSICION 0");
    printf("\nVERSION ARREGLO:\n");
    for (int i = 0; i < h.cantidad; i++) {
        printf("[%d] ", ((Archivo*)h.datos[i])->paginas);
    }
    printf("\n\nArbol:(Rotado):\n");
    if (h.cantidad == 0) printf(" (Cola vacia)\n");
    else imprimirComoArbol(h, 0, 0);
}

void imprimirComoArbol(Heap h, int idx, int nivel) {
    if (idx >= h.cantidad) return;

    //(Derecha - Raíz - Izquierda)
    imprimirComoArbol(h, 2 * idx + 2, nivel + 1);

    for (int i = 0; i < nivel; i++) printf("      ");
    printf("|--[%d]\n", ((Archivo*)h.datos[idx])->paginas);

    imprimirComoArbol(h, 2 * idx + 1, nivel + 1);
}
