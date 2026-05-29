#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <iostring.h>
#include <arbol.h>

/* --- Prototipos Base --- */
int* crearEntero(int dato);
int compararEntero(void* a, void* b);
void imprimirEntero(void *a);

int alturaNodo(NodoA *raiz);
void calcularProfundidades(Arbol *arbol, char nombre);
int compararEstructura(NodoA *a, NodoA *b);
void inordenArreglo(NodoA *raiz, int *arr, int *indice);
int compararDatos(Arbol *a, Arbol *b);
NodoA* eliminarNodoEntero(NodoA *raiz, int valor, int *eliminado);
void extraerMitades(int *arr1, int *arr2, int longitud, int *i);
void equilibrarArbol(Arbol *arbol);

int main(void)
{
    // Inicialización de los árboles
    Arbol arbolA, arbolB;
    arbolA.raiz = NULL; arbolA.cantidad = 0;
    arbolA.imprimir = &imprimirEntero; arbolA.comparar = &compararEntero; arbolA.liberar = &free;

    arbolB.raiz = NULL; arbolB.cantidad = 0;
    arbolB.imprimir = &imprimirEntero; arbolB.comparar = &compararEntero; arbolB.liberar = &free;

    int opcion = 0, valor, eliminado;

    do {
        printf("\n--- MENU DE ARBOLES BINARIOS ---");
        printf("\n1. Insertar en Arbol A");
        printf("\n2. Insertar en Arbol B");
        printf("\n3. Comparar Arboles");
        printf("\n4. Eliminar dato en Arbol A");
        printf("\n5. Eliminar dato en Arbol B");
        printf("\n6. Profundidades");
        printf("\n7. Equilibrar arboles");
        printf("\n8. Mostrar Arboles");
        printf("\n9. Terminar programa");
        printf("\nOpcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: // Insertar en Arbol A
                printf("Ingrese valor para Arbol A: ");
                scanf("%d", &valor);
                insertarArbol(&arbolA, crearEntero(valor));
                break;

            case 2: // Insertar en Arbol B
                printf("Ingrese valor para Arbol B: ");
                scanf("%d", &valor);
                insertarArbol(&arbolB, crearEntero(valor));
                break;

            case 3: // Comparar Arboles
                if (compararDatos(&arbolA, &arbolB)) {
                    printf("\nResultados: Datos Iguales\n");
                    if (compararEstructura(arbolA.raiz, arbolB.raiz)) {
                        printf("Misma Estructura\n");
                    } else {
                        printf("Diferente Estructura\n");
                    }
                } else {
                    printf("\nResultados: Datos Diferentes\n");
                }
                break;

            case 4: // Eliminar dato en Arbol A
                printf("Valor a eliminar en Arbol A: ");
                scanf("%d", &valor);
                eliminado = 0;
                arbolA.raiz = eliminarNodoEntero(arbolA.raiz, valor, &eliminado);
                if(eliminado) { arbolA.cantidad--; printf("Dato eliminado.\n"); }
                else printf("Dato no encontrado.\n");
                break;

            case 5: // Eliminar dato en Arbol B
                printf("Valor a eliminar en Arbol B: ");
                scanf("%d", &valor);
                eliminado = 0;
                arbolB.raiz = eliminarNodoEntero(arbolB.raiz, valor, &eliminado);
                if(eliminado) { arbolB.cantidad--; printf("Dato eliminado.\n"); }
                else printf("Dato no encontrado.\n");
                break;

            case 6: // Profundidades
                calcularProfundidades(&arbolA, 'A');
                calcularProfundidades(&arbolB, 'B');
                break;

            case 7: // Equilibrar arboles
                printf("\nEquilibrando Arbol A...\n");
                equilibrarArbol(&arbolA);
                printf("Equilibrando Arbol B...\n");
                equilibrarArbol(&arbolB);
                break;

            case 8: // Mostrar Arboles
                printf("\n--- ARBOL A ---\n");
                imprimirArbol(arbolA);
                printf("\n--- ARBOL B ---\n");
                imprimirArbol(arbolB);
                break;

            case 9: // Terminar programa
                printf("\nLiberando memoria (POSTORDEN)...\n");
                eliminarArbol(&arbolA);
                eliminarArbol(&arbolB);
                printf("\nFIN DE PROGRAMA\n");
                break;

            default:
                printf("\nOpcion no valida.\n");
        }
    } while(opcion != 9);

    return 0;
}

/* ==========================================
   FUNCIONES DE LÓGICA DE ÁRBOLES
   ========================================== */

// Calcular altura y factor de equilibrio B = HD - HI
int alturaNodo(NodoA *raiz) {
    if (raiz == NULL) return 0;
    int altIzq = alturaNodo(raiz->izq);
    int altDch = alturaNodo(raiz->dch);
    return (altIzq > altDch ? altIzq : altDch) + 1;
}

void calcularProfundidades(Arbol *arbol, char nombre) {
    int altIzq = arbol->raiz ? alturaNodo(arbol->raiz->izq) : 0;
    int altDch = arbol->raiz ? alturaNodo(arbol->raiz->dch) : 0;
    int factorEquilibrio = altDch - altIzq; // B = HD - HI

    printf("\nArbol %c:", nombre);
    printf("\n H_I = %d", altIzq);
    printf("\n H_D = %d", altDch);
    printf("\n Factor B = %d", factorEquilibrio);

    if (factorEquilibrio >= -1 && factorEquilibrio <= 1) {
        printf(" (El arbol esta equilibrado)\n");
    } else {
        printf(" (El arbol NO esta equilibrado)\n");
    }
}

// Comparación de Estructura Recursiva
int compararEstructura(NodoA *a, NodoA *b) {
    if (a == NULL && b == NULL) return 1;
    if (a == NULL || b == NULL) return 0;
    return compararEstructura(a->izq, b->izq) && compararEstructura(a->dch, b->dch);
}

// Extraer elementos en inorden para comparar datos ordenados
void inordenArreglo(NodoA *raiz, int *arr, int *indice) {
    if (raiz != NULL) {
        inordenArreglo(raiz->izq, arr, indice);
        arr[*indice] = *(int*)raiz->dato;
        (*indice)++;
        inordenArreglo(raiz->dch, arr, indice);
    }
}

// Compara si los datos son iguales independientemente de la estructura
int compararDatos(Arbol *a, Arbol *b) {
    if (a->cantidad != b->cantidad) return 0;
    if (a->cantidad == 0) return 1;

    int *arrA = (int*)malloc(a->cantidad * sizeof(int));
    int *arrB = (int*)malloc(b->cantidad * sizeof(int));
    int indA = 0, indB = 0;

    inordenArreglo(a->raiz, arrA, &indA);
    inordenArreglo(b->raiz, arrB, &indB);

    int iguales = 1;
    for (int i = 0; i < a->cantidad; i++) {
        if (arrA[i] != arrB[i]) {
            iguales = 0; break;
        }
    }

    free(arrA); free(arrB);
    return iguales;
}

// Balanceo por reconstrucción total
void extraerMitades(int *arr1, int *arr2, int longitud, int *i) {
    if(longitud <= 0) return;
    int mitad = longitud/2;
    arr2[*i] = arr1[mitad];
    (*i)++;
    extraerMitades(arr1, arr2, mitad, i);
    extraerMitades(arr1 + mitad + 1, arr2, longitud - 1 - mitad, i);
}

void equilibrarArbol(Arbol *arbol) {
    if (arbol->cantidad <= 2) return; // Ya está balanceado si tiene 0, 1 o 2 nodos

    // 1. Extraer a un arreglo ordenado
    int *arrOrdenado = (int*)malloc(arbol->cantidad * sizeof(int));
    int *arrBalanceado = (int*)malloc(arbol->cantidad * sizeof(int));
    int indice = 0, idxBalanceado = 0;

    inordenArreglo(arbol->raiz, arrOrdenado, &indice);

    // 2. Generar el arreglo con el orden correcto para que quede equilibrado
    extraerMitades(arrOrdenado, arrBalanceado, arbol->cantidad, &idxBalanceado);

    // 3. Vaciar el árbol actual y reinsertar
    eliminarArbol(arbol); // Limpia y libera
    arbol->raiz = NULL; arbol->cantidad = 0;

    for (int i = 0; i < idxBalanceado; i++) {
        insertarArbol(arbol, crearEntero(arrBalanceado[i]));
    }

    free(arrOrdenado);
    free(arrBalanceado);
}

// Eliminación estándar de un nodo en un ABB
NodoA* eliminarNodoEntero(NodoA *raiz, int valor, int *eliminado) {
    if (raiz == NULL) return raiz;

    int valorRaiz = *(int*)raiz->dato;

    if (valor < valorRaiz) {
        raiz->izq = eliminarNodoEntero(raiz->izq, valor, eliminado);
    } else if (valor > valorRaiz) {
        raiz->dch = eliminarNodoEntero(raiz->dch, valor, eliminado);
    } else {
        *eliminado = 1; // Se encontró el nodo
        // Caso 1: Sin hijos o un solo hijo
        if (raiz->izq == NULL) {
            NodoA *temp = raiz->dch;
            free(raiz->dato); free(raiz);
            return temp;
        } else if (raiz->dch == NULL) {
            NodoA *temp = raiz->izq;
            free(raiz->dato); free(raiz);
            return temp;
        }
        // Caso 2: Dos hijos. Buscar el sucesor inorden (menor del subárbol derecho)
        NodoA *temp = raiz->dch;
        while (temp && temp->izq != NULL) temp = temp->izq;

        *(int*)raiz->dato = *(int*)temp->dato; // Copiar dato del sucesor
        raiz->dch = eliminarNodoEntero(raiz->dch, *(int*)temp->dato, eliminado); // Eliminar sucesor
        *eliminado = 1; // Ajuste para no alterar el conteo general al volver
    }
    return raiz;
}

/* ==========================================
   FUNCIONES AUXILIARES  práctica anterior
   ========================================== */
int* crearEntero(int dato) {
    int *nuevo = malloc(sizeof(int));
    if(nuevo) *nuevo = dato;
    return nuevo;
}

int compararEntero(void* a, void* b) {
    int *aa = a, *bb = b;
    if(*aa == *bb) return 0;
    return (*aa < *bb) ? -1 : 1;
}

void imprimirEntero(void *a) {
    if(a) printf("%d", *(int*)a);
}
