#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <iostring.h>
#include <arbol.h>

/* --- Prototipos --- */
void abortar(void);
int* crearEntero(int);
int compararEntero(void*,void*);
void imprimirEntero(void*);
void imprimirNodo(void*);
void imprimirResultado(Resultado resultado);
NodoA* buscarManual(NodoA* raiz, void* valor, int (*comparar)(void*,void*));
void flipArbol(NodoA* raiz);
void extraerMitades(int *arr1,int *arr2,int longitud,int *i);
void myprintf(char *msg,char *formato,...);
void ordenIterativo(NodoA *raiz,void (*imprimir)(void*));

int main(void)
{
    Arbol arbol;
    arbol.raiz = NULL;
    arbol.cantidad = 0;
    arbol.imprimir = &imprimirEntero;
    arbol.comparar = &compararEntero;
    arbol.liberar = &free;

    // Inserciones iniciales
    insertarArbol(&arbol, crearEntero(7));
    insertarArbol(&arbol, crearEntero(4));
    insertarArbol(&arbol, crearEntero(2));
    insertarArbol(&arbol, crearEntero(8));
    insertarArbol(&arbol, crearEntero(9));
    insertarArbol(&arbol, crearEntero(6));
    insertarArbol(&arbol, crearEntero(1));
    insertarArbol(&arbol, crearEntero(5));

    printf("\n--- ARBOL ORIGINAL ---");
    printf("\nVisualizacion:\n");
    imprimirArbol(arbol);

    printf("\nOrden Iterativo: ");
    ordenIterativo(arbol.raiz, imprimirNodo);

    // buscar antes del Flip ---
    int valorBusqueda = 12;
    printf("\n\nvalor %d antes del flip", valorBusqueda);
    NodoA* encontrado = buscarManual(arbol.raiz, &valorBusqueda, compararEntero);

    if(encontrado != NULL) {
        printf("\n ENCONTRADO: ");
        imprimirEntero(encontrado->dato);
    } else {
        printf("\n NO ENCONTRADO");
    }

    flipArbol(arbol.raiz);

    printf("\n--- ARBOL INVERTIDO ---");
    printf("\nVisualizacion:\n");
    imprimirArbol(arbol);

    eliminarArbol(&arbol);

    // Arreglos
    int arr1[9] = {1,4,5,6,7,8,9,10,11};
    int arr2[9];
    int idx = 0;
    extraerMitades(arr1, arr2, 9, &idx);

    printf("\n\nResultado extraerMitades:");
    for(int j = 0; j < 9; j++) {
        printf(" %d", arr2[j]);
    }

    printf("\n\n FIN DE PROGRAMA\n");
    return 0;
}



// Flip arbol
void flipArbol(NodoA* raiz)
{
    if(raiz == NULL)
        return;

    // Intercambio
    NodoA* temp = raiz->izq;
    raiz->izq = raiz->dch;
    raiz->dch = temp;

    // Llamada recursiva para los hijos
    flipArbol(raiz->izq);
    flipArbol(raiz->dch);
}

//Búsqueda Manual
NodoA* buscarManual(NodoA* raiz, void* valor, int (*comparar)(void*,void*))
{
    if(raiz == NULL)
        return NULL;

    int comp = comparar(valor, raiz->dato);

    if(comp == 0)
        return raiz;

    // Si es menor busca en la izquierda, si es mayor en la derecha
    if(comp < 0)
        return buscarManual(raiz->izq, valor, comparar);
    else
        return buscarManual(raiz->dch, valor, comparar);
}

void ordenIterativo(NodoA *raiz, void (*imprimir)(void*))
{
    Pila pila = {NULL, 0, -1, imprimir, NULL};
    if(!raiz) return;

    NodoA* aux = raiz;
    while(aux || !pilaVacia(pila))
    {
        while(aux)
        {
            pushDato(&pila, aux);
            aux = aux->izq;
        }
        aux = popDato(&pila);
        imprimir(aux);
        printf(" ");
        aux = aux->dch;
    }
    vaciarPila(&pila);
}

void extraerMitades(int *arr1, int *arr2, int longitud, int *i)
{
    if(longitud <= 0)
        return;
    int mitad = longitud/2;
    arr2[*i] = arr1[mitad];
    (*i)++;
    extraerMitades(arr1, arr2, mitad, i);
    extraerMitades(arr1 + mitad + 1, arr2, longitud - 1 - mitad, i);
}

int* crearEntero(int dato)
{
    int *nuevo = malloc(sizeof(int));
    if(nuevo) *nuevo = dato;
    return nuevo;
}

int compararEntero(void* a, void* b)
{
    int *aa = a, *bb = b;
    if(*aa == *bb) return 0;
    return (*aa < *bb) ? -1 : 1;
}

void imprimirEntero(void *a)
{
    if(a) printf("%d", *(int*)a);
}

void imprimirNodo(void *a)
{
    NodoA *node = a;
    imprimirEntero(node->dato);
}

void imprimirResultado(Resultado resultado)
{
    if(resultado.nodo)
    {
        if(resultado.padre)
        {
            printf("\n PADRE: ");
            imprimirEntero(resultado.padre->dato);
            printf("\n RAMA: %s", (resultado.rama) ? "DERECHA" : "IZQUIERDA");
        }
        printf("\n Nodo: ");
        imprimirEntero(resultado.nodo->dato);
    }
    else
        printf("\n NODO NO ENCONTRADO");
}

void abortar(void)
{
    if(getchar() == 'e') exit(0);
}

void myprintf(char* msg, char *formato, ...)
{
    va_list args;
    va_start(args, formato);
    printf("\n %s ", msg);
    char *ptr = formato;
    while(*ptr != '\0')
    {
        switch(*ptr)
        {
            case 'd': printf("%d ", va_arg(args, int)); break;
            case 'c': printf("%c ", va_arg(args, int)); break;
            case 'f': printf("%f ", va_arg(args, double)); break;
        }
        ptr++;
    }
    va_end(args);
}
