#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostring.h>
#include <HashQ.h>
#include "Libro.h"

typedef struct NodoArbol {
    Libro *libro;
    struct NodoArbol *izq;
    struct NodoArbol *dch;
} NodoArbol;

int compararISBN(void *a, void *b);
int compararTitulo(void *a, void *b);
int compararAutor(void *a, void *b);
int compararAnio(void *a, void *b);

int foldingISBN(void *data);
int foldingTitulo(void *data);
int foldingAutor(void *data);
int foldingAnio(void *data);
void imprimirElemento(void *data);

void insertarArbolTitulo(NodoArbol **raiz, Libro *lib);
void insertarArbolAutor(NodoArbol **raiz, Libro *lib);
void insertarArbolFecha(NodoArbol **raiz, Libro *lib);
void inordenArbol(NodoArbol *raiz);
void liberarArbol(NodoArbol *raiz);

void buscarExactaAutorHash(HashQTable *tabla, NodoArbol **raiz);
void buscarExactaAnioHash(HashQTable *tabla, NodoArbol **raiz);
void buscarRangoTituloHash(HashQTable *tabla, NodoArbol **raiz);
void buscarRangoAutorHash(HashQTable *tabla, NodoArbol **raiz);
void buscarRangoFechaHash(HashQTable *tabla, NodoArbol **raiz);
void eliminarLibroPorISBN(HashQTable *hISBN, HashQTable *hTitulo, HashQTable *hAutor, HashQTable *hAnio);

void inicializarHash(HashQTable *h, int len, int min, int max,
                     int(*folding)(void*), void(*imprimir)(void*),
                     int(*comparar)(void*,void*))
{
    h->table = calloc(len, sizeof(void*));
    h->len = len;
    h->cant = 0;
    h->min = min;
    h->max = max;
    h->fixed = (min + max) / 2;
    h->folding = folding;
    h->imprimir = imprimir;
    h->comparar = comparar;
    h->verbose = 0;
}

void cargarLibros(HashQTable *hISBN, HashQTable *hTitulo, HashQTable *hAutor, HashQTable *hAnio, Acervo *acervo)
{
    *acervo = obtener_libros();
    for(int i = 0; i < acervo->cantidad; i++) {
        Libro *lib = &acervo->libros[i];
        fn_insertKey(hISBN, lib);
        fn_insertKey(hTitulo, lib);
        fn_insertKey(hAutor, lib);
        fn_insertKey(hAnio, lib);
    }
}

void reiniciarHash(HashQTable *h, int len, int min, int max,
                   int(*folding)(void*), void(*imprimir)(void*),
                   int(*comparar)(void*,void*))
{
    free(h->table);
    inicializarHash(h, len, min, max, folding, imprimir, comparar);
}

int main(void)
{
    HashQTable hashISBN, hashTitulo, hashAutor, hashAnio;

    inicializarHash(&hashISBN,   53, 30, 60, foldingISBN,   imprimirElemento, compararISBN);
    inicializarHash(&hashTitulo, 53, 30, 60, foldingTitulo, imprimirElemento, compararTitulo);
    inicializarHash(&hashAutor,  53, 30, 60, foldingAutor,  imprimirElemento, compararAutor);
    inicializarHash(&hashAnio,   53, 30, 60, foldingAnio,   imprimirElemento, compararAnio);

    Acervo acervo;
    cargarLibros(&hashISBN, &hashTitulo, &hashAutor, &hashAnio, &acervo);
    printf("\nLibros cargados: %d\n\n", acervo.cantidad);//nota para mi: esta opcion solo la utilice para un debug

    int opcion;
    do {
        printf("      SISTEMA BIBLIOTECA\n");
        printf("1.  Mostrar todos los libros\n");
        printf("2.  Buscar libro por ISBN (Exacto)\n");
        printf("3.  Buscar libro por Titulo (Exacto)\n");
        printf("4.  Buscar libros por Autor (Exacto - Ordenados por Fecha)\n");
        printf("5.  Buscar libros por Anio especifico (Exacto - Ordenados por Titulo)\n");
        printf("6.  Buscar libros por rango de iniciales de Titulo\n");
        printf("7.  Buscar libros por rango de iniciales de Autor\n");
        printf("8.  Buscar libros por rango de Anios de publicacion\n");
        printf("9.  Eliminar libro por ISBN\n");
        printf("10. Reiniciar sistema\n");
        printf("0.  Salir del programa\n");
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) break;
        getchar();

        switch(opcion) {
            case 1: {
                printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                printf("-------------------------------------------------------------------------------------------------------\n");
                for(int i = 0; i < hashISBN.len; i++) {
                    if(hashISBN.table[i] != NULL && hashISBN.table[i] != &hashISBN) {
                        imprimirLibro(hashISBN.table[i]);
                        printf("\n");
                    }
                }
                break;
            }

            case 2: {
                char isbnBuscar[MAX_ISBN];
                printf("Ingrese el ISBN: ");
                fgets(isbnBuscar, MAX_ISBN, stdin);
                isbnBuscar[strcspn(isbnBuscar, "\n")] = 0;
                Libro buscado;
                strcpy(buscado.isbn, isbnBuscar);
                Libro *res = (Libro*)fn_searchKey(&hashISBN, &buscado);
                if(res) {
                    printf("\nEncontrado por ISBN:\n");
                    imprimirLibro(res);
                    printf("\n\n");
                } else {
                    printf("\n No se encontro el ISBN: %s\n\n", isbnBuscar);
                }
                break;
            }

            case 3: {
                char tituloBuscar[MAX_TITULO];
                printf("Ingrese el Titulo ");
                fgets(tituloBuscar, MAX_TITULO, stdin);
                tituloBuscar[strcspn(tituloBuscar, "\n")] = 0;
                Libro buscado;
                strcpy(buscado.titulo, tituloBuscar);
                Libro *res = (Libro*)fn_searchKey(&hashTitulo, &buscado);
                if(res) {
                    printf("\n Encontrado por Titulo:\n");
                    imprimirLibro(res);
                    printf("\n\n");
                } else {
                    printf("\n No se encontro el libro: \"%s\"\n\n", tituloBuscar);
                }
                break;
            }

            case 4: {
                NodoArbol *raiz = NULL;
                buscarExactaAutorHash(&hashAutor, &raiz);
                if(raiz) {
                    printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                    printf("-------------------------------------------------------------------------------------------------------\n");
                    inordenArbol(raiz);
                    printf("\n");
                    liberarArbol(raiz);
                } else {
                    printf("\n No se encontraron libros para el autor indicado.\n\n");
                }
                break;
            }

            case 5: {
                NodoArbol *raiz = NULL;
                buscarExactaAnioHash(&hashAnio, &raiz);
                if(raiz) {
                    printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                    printf("-------------------------------------------------------------------------------------------------------\n");
                    inordenArbol(raiz);
                    printf("\n");
                    liberarArbol(raiz);
                } else {
                    printf("\nNo se encontraron libros para el anio indicado.\n\n");
                }
                break;
            }

            case 6: {
                NodoArbol *raiz = NULL;
                buscarRangoTituloHash(&hashTitulo, &raiz);
                if(raiz) {
                    printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                    printf("-------------------------------------------------------------------------------------------------------\n");
                    inordenArbol(raiz);
                    printf("\n");
                    liberarArbol(raiz);
                } else {
                    printf("\nNo se encontraron libros en el rango indicado.\n\n");
                }
                break;
            }

            case 7: {
                NodoArbol *raiz = NULL;
                buscarRangoAutorHash(&hashAutor, &raiz);
                if(raiz) {
                    printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                    printf("-------------------------------------------------------------------------------------------------------\n");
                    inordenArbol(raiz);
                    printf("\n");
                    liberarArbol(raiz);
                } else {
                    printf("\n[INFO] No se encontraron libros en el rango indicado.\n\n");
                }
                break;
            }

            case 8: {
                NodoArbol *raiz = NULL;
                buscarRangoFechaHash(&hashAnio, &raiz);
                if(raiz) {
                    printf("\n%-18s | %-41s | %-25s | %s\n", "ISBN", "TITULO", "AUTOR", "ANIO | ESTADO");
                    printf("-------------------------------------------------------------------------------------------------------\n");
                    inordenArbol(raiz);
                    printf("\n");
                    liberarArbol(raiz);
                } else {
                    printf("\n No se encontraron libros en el rango \n\n");
                }
                break;
            }

            case 9:
                eliminarLibroPorISBN(&hashISBN, &hashTitulo, &hashAutor, &hashAnio);
                break;

            case 10:
                reiniciarHash(&hashISBN,   53, 30, 60, foldingISBN,   imprimirElemento, compararISBN);
                reiniciarHash(&hashTitulo, 53, 30, 60, foldingTitulo, imprimirElemento, compararTitulo);
                reiniciarHash(&hashAutor,  53, 30, 60, foldingAutor,  imprimirElemento, compararAutor);
                reiniciarHash(&hashAnio,   53, 30, 60, foldingAnio,   imprimirElemento, compararAnio);
                cargarLibros(&hashISBN, &hashTitulo, &hashAutor, &hashAnio, &acervo);
                printf("\nSistema reiniciado con todos los libros.\n\n");
                break;

            case 0:
                printf("\nSaliendo \n");
                break;

            default:
                printf("\nOpcion no valida\n\n");
        }
    } while(opcion != 0);

    free(hashISBN.table);
    free(hashTitulo.table);
    free(hashAutor.table);
    free(hashAnio.table);

    printf("\n\n FIN DE PROGRAMA\n");
    return 0;
}
