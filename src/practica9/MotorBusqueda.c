#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <HashQ.h>
#include "Libro.h"


typedef struct NodoArbol {
    Libro *libro;
    struct NodoArbol *izq;
    struct NodoArbol *dch;
} NodoArbol;

int compararISBN(void *a, void *b) {
    Libro *lib1 = (Libro*)a;
    Libro *lib2 = (Libro*)b;
    return strcmp(lib1->isbn, lib2->isbn);
}

int compararTitulo(void *a, void *b) {
    Libro *lib1 = (Libro*)a;
    Libro *lib2 = (Libro*)b;
    return strcmp(lib1->titulo, lib2->titulo);
}

int compararAutor(void *a, void *b) {
    Libro *lib1 = (Libro*)a;
    Libro *lib2 = (Libro*)b;
    return strcmp(lib1->autor, lib2->autor);
}

int compararAnio(void *a, void *b) {
    Libro *lib1 = (Libro*)a;
    Libro *lib2 = (Libro*)b;
    if(lib1->fecha < lib2->fecha) return -1;
    if(lib1->fecha > lib2->fecha) return 1;
    return 0;
}

int foldingISBN(void *data) {
    Libro *libro = (Libro*)data;
    char *str = libro->isbn;
    int key = 0, sector = 1, sumatoria = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '-') continue;
        sumatoria += tolower(str[i]);
        if(i != 0 && (i+1)%3 == 0) {
            key += sector * sumatoria;
            sumatoria = 0;
            sector++;
        }
    }
    key += sector * sumatoria;
    return key;
}

int foldingTitulo(void *data) {
    Libro *libro = (Libro*)data;
    char *str = libro->titulo;
    int key = 0, sector = 1, sumatoria = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        sumatoria += tolower(str[i]);
        if(i != 0 && (i+1)%4 == 0) {
            key += sector * sumatoria;
            sumatoria = 0;
            sector++;
        }
    }
    key += sector * sumatoria;
    return key;
}

int foldingAutor(void *data) {
    Libro *libro = (Libro*)data;
    char *str = libro->autor;
    int key = 0, sector = 1, sumatoria = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        sumatoria += tolower(str[i]);
        if(i != 0 && (i+1)%3 == 0) {
            key += sector * sumatoria;
            sumatoria = 0;
            sector++;
        }
    }
    key += sector * sumatoria;
    return key;
}

int foldingAnio(void *data) {
    Libro *libro = (Libro*)data;
    return libro->fecha;
}

void imprimirElemento(void *data) {
    Libro *libro = (Libro*)data;
    printf("%s", libro->isbn);
}

void insertarArbolTitulo(NodoArbol **raiz, Libro *lib) {
    if (!(*raiz)) {
        *raiz = malloc(sizeof(NodoArbol));
        (*raiz)->libro = lib;
        (*raiz)->izq = NULL;
        (*raiz)->dch = NULL;
        return;
    }
    if (strcmp(lib->titulo, (*raiz)->libro->titulo) < 0) {
        insertarArbolTitulo(&((*raiz)->izq), lib);
    } else {
        insertarArbolTitulo(&((*raiz)->dch), lib);
    }
}

void insertarArbolAutor(NodoArbol **raiz, Libro *lib) {
    if (!(*raiz)) {
        *raiz = malloc(sizeof(NodoArbol));
        (*raiz)->libro = lib;
        (*raiz)->izq = NULL;
        (*raiz)->dch = NULL;
        return;
    }
    if (toupper(lib->autor[0]) < toupper((*raiz)->libro->autor[0])) {
        insertarArbolAutor(&((*raiz)->izq), lib);
    } else {
        insertarArbolAutor(&((*raiz)->dch), lib);
    }
}

void insertarArbolFecha(NodoArbol **raiz, Libro *lib) {
    if (!(*raiz)) {
        *raiz = malloc(sizeof(NodoArbol));
        (*raiz)->libro = lib;
        (*raiz)->izq = NULL;
        (*raiz)->dch = NULL;
        return;
    }
    if (lib->fecha < (*raiz)->libro->fecha) {
        insertarArbolFecha(&((*raiz)->izq), lib);
    } else if (lib->fecha > (*raiz)->libro->fecha) {
        insertarArbolFecha(&((*raiz)->dch), lib);
    } else {
        if (strcmp(lib->titulo, (*raiz)->libro->titulo) < 0) {
            insertarArbolFecha(&((*raiz)->izq), lib);
        } else {
            insertarArbolFecha(&((*raiz)->dch), lib);
        }
    }
}

void inordenArbol(NodoArbol *raiz) {
    if (raiz) {
        inordenArbol(raiz->izq);
        imprimirLibro(raiz->libro);
        printf("\n");
        inordenArbol(raiz->dch);
    }
}

void liberarArbol(NodoArbol *raiz) {
    if (raiz) {
        liberarArbol(raiz->izq);
        liberarArbol(raiz->dch);
        free(raiz);
    }
}

void buscarExactaAutorHash(HashQTable *tabla, NodoArbol **raiz) {
    char autorBuscar[MAX_AUTOR];
    printf("Ingrese el Autor exacto a buscar: ");
    fgets(autorBuscar, MAX_AUTOR, stdin);
    autorBuscar[strcspn(autorBuscar, "\n")] = 0;

    for (int i = 0; i < tabla->len; i++) {
        if (tabla->table[i] != NULL && tabla->table[i] != tabla) {
            Libro *lib = (Libro*)tabla->table[i];
            if (strcmp(lib->autor, autorBuscar) == 0) {
                insertarArbolFecha(raiz, lib);
            }
        }
    }
}

void buscarExactaAnioHash(HashQTable *tabla, NodoArbol **raiz) {
    int anioBuscar;
    printf("Ingrese el Anio exacto a buscar: ");
    scanf("%d", &anioBuscar);
    getchar();

    for (int i = 0; i < tabla->len; i++) {
        if (tabla->table[i] != NULL && tabla->table[i] != tabla) {
            Libro *lib = (Libro*)tabla->table[i];
            if (lib->fecha == anioBuscar) {
                insertarArbolTitulo(raiz, lib);
            }
        }
    }
}

void buscarRangoTituloHash(HashQTable *tabla, NodoArbol **raiz) {
    char inf, sup;
    printf("Ingrese limite inferior (Inicial del Titulo): ");
    scanf(" %c", &inf);
    printf("Ingrese limite superior (Inicial del Titulo): ");
    scanf(" %c", &sup);
    inf = toupper(inf);//Validacion mayus, minus
    sup = toupper(sup);

    for (int i = 0; i < tabla->len; i++) { //Recorre la tabla
        if (tabla->table[i] != NULL && tabla->table[i] != tabla) { //Posicion vacia?
            Libro *lib = (Libro*)tabla->table[i];
            char inicial = toupper(lib->titulo[0]);
            if (inicial >= inf && inicial <= sup) { //Dentro del rango?
                insertarArbolTitulo(raiz, lib);
            }
        }
    }
}

void buscarRangoAutorHash(HashQTable *tabla, NodoArbol **raiz) {
    char inf, sup;
    printf("Ingrese limite inferior (Inicial del Autor): ");
    scanf(" %c", &inf);
    printf("Ingrese limite superior (Inicial del Autor): ");
    scanf(" %c", &sup);
    inf = toupper(inf);
    sup = toupper(sup);

    for (int i = 0; i < tabla->len; i++) {
        if (tabla->table[i] != NULL && tabla->table[i] != tabla) {
            Libro *lib = (Libro*)tabla->table[i];
            char inicial = toupper(lib->autor[0]);
            if (inicial >= inf && inicial <= sup) {
                insertarArbolAutor(raiz, lib);
            }
        }
    }
}

void buscarRangoFechaHash(HashQTable *tabla, NodoArbol **raiz) {
    int inf, sup;
    printf("Ingrese limite inferior (Anio): ");
    scanf("%d", &inf);
    printf("Ingrese limite superior (Anio): ");
    scanf("%d", &sup);

    for (int i = 0; i < tabla->len; i++) {
        if (tabla->table[i] != NULL && tabla->table[i] != tabla) {
            Libro *lib = (Libro*)tabla->table[i];
            if (lib->fecha >= inf && lib->fecha <= sup) {
                insertarArbolFecha(raiz, lib);
            }
        }
    }
}

void eliminarLibroPorISBN(HashQTable *hISBN, HashQTable *hTitulo, HashQTable *hAutor, HashQTable *hAnio) {
    char isbnBuscar[MAX_ISBN];
    printf("Ingrese el ISBN del libro a eliminar: ");
    fgets(isbnBuscar, MAX_ISBN, stdin);
    isbnBuscar[strcspn(isbnBuscar, "\n")] = 0;

    Libro buscado;
    strcpy(buscado.isbn, isbnBuscar);

    Libro *encontrado = (Libro*)fn_searchKey(hISBN, &buscado);
    if (!encontrado) {
        printf("\n No se encontro ningun libro con el ISBN: %s\n\n", isbnBuscar);
        return;
    }

    fn_deleteKey(hTitulo, encontrado);
    fn_deleteKey(hAutor, encontrado);
    fn_deleteKey(hAnio, encontrado);
    fn_deleteKey(hISBN, encontrado);

    printf("\nEl libro \"%s\" ha sido eliminado de todos los indices.\n\n", encontrado->titulo);
}
