#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"
#include "expresion.h"
#include "variable.h"
#include "arbol.h"

//Prototipos P6
int esValida(const char* exp);
void infixAPostfix(const char* infix, char* postfix);
int prioridadOp(char c); // Se renombró para evitar conflictos
int esOperando(char c);

// Nuevos
Arbol postfixAArbol(const char* postfix);
void liberarArbol(NodoA* raiz);

int main(void)
{
    int opcion;
    char infix[100] = "";
    char postfix[100] = "";
    Arbol arbolExpresion;
    arbolExpresion.raiz = NULL;
    int expresionValida = 0;

    do {
        printf("\nExpresion\n");
        if(expresionValida) printf("Última capturada: %s\n", infix);

        printf("[0] CAPTURAR EXPRESION\n");
        printf("[1] IMPRIMIR ARBOL DE EXPRESION\n");
        printf("[2] EVALUAR EXPRESION\n");
        printf("[3] TERMINAR PROGRAMA\n");
        printf("Elige una opción: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = -1;
        }
        getchar();

        switch(opcion) {
            case 0:
                printf("Captura expresion Infix: ");
                fgets(infix, sizeof(infix), stdin);
                infix[strcspn(infix, "\n")] = 0;

                if (esValida(infix)) {
                    infixAPostfix(infix, postfix);
                    printf("-> Expresion valida (Postfix: %s)\n", postfix);
                    expresionValida = 1;

                    if (arbolExpresion.raiz != NULL) {
                        liberarArbol(arbolExpresion.raiz);
                    }
                    arbolExpresion = postfixAArbol(postfix);
                } else {
                    printf("-> ERROR: La expresion no es valida.\n");
                    expresionValida = 0;
                }
                break;

            case 1:
                if (expresionValida && arbolExpresion.raiz != NULL) {
                    printf("\nArbol de expresion:\n");
                    imprimirArbol(arbolExpresion);
                } else {
                    printf("Primero debes capturar una expresión válida.\n");
                }
                break;

            case 2:
                if (expresionValida && arbolExpresion.raiz != NULL) {
                    evaluar(arbolExpresion);
                } else {
                    printf("Captura una expresion valida primero.\n");
                }
                break;

            case 3:
                printf("\nFin. Se libero memoria \n");
                if (arbolExpresion.raiz != NULL) {
                    liberarArbol(arbolExpresion.raiz);
                }
                break;
            default:
                printf("Op. no valida D:\n");
        }
    } while(opcion != 3);
    return 0;
}

Arbol postfixAArbol(const char* postfix) {
    Pila p = {NULL, 0, 100, NULL, NULL};
    int cantidadNodos = 0;
    for(int i = 0; postfix[i] != '\0'; i++) {
        char actual = postfix[i];
        Variable* var = crearVariable(actual);
        NodoA* nuevoNodo = (NodoA*)malloc(sizeof(NodoA));
        nuevoNodo->dato = var;
        nuevoNodo->izq = NULL;
        nuevoNodo->dch = NULL;
        cantidadNodos++;
        if (esOperando(actual)) {
            pushDato(&p, nuevoNodo);
        } else {
            nuevoNodo->dch = (NodoA*)popDato(&p);
            nuevoNodo->izq = (NodoA*)popDato(&p);
            pushDato(&p, nuevoNodo);
        }
    }
    Arbol arbol;
    arbol.raiz = (NodoA*)popDato(&p);
    arbol.cantidad = cantidadNodos;
    arbol.comparar = compararVariable;
    arbol.imprimir = imprimirVariable;
    return arbol;
}

void liberarArbol(NodoA* raiz) {
    if (raiz == NULL) return;
    liberarArbol(raiz->izq);
    liberarArbol(raiz->dch);
    if(raiz->dato) free(raiz->dato);
    free(raiz);
}

int esValida(const char* exp) {
    Pila p = {NULL, 0, 100, NULL, NULL};
    for (int i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(') {
            char* parenthesis = (char*)malloc(sizeof(char));
            *parenthesis = exp[i];
            pushDato(&p, parenthesis);
        } else if (exp[i] == ')') {
            if (pilaVacia(p)) return 0;
            free(popDato(&p));
        }
    }
    int valida = pilaVacia(p);
    while(!pilaVacia(p)) free(popDato(&p));
    return valida;
}

void infixAPostfix(const char* infix, char* postfix) {
    Pila p = {NULL, 0, 100, NULL, NULL};
    int j = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        char actual = infix[i];
        if (esOperando(actual)) {
            postfix[j++] = actual;
        } else if (actual == '(') {
            char* item = (char*)malloc(sizeof(char));
            *item = actual;
            pushDato(&p, item);
        } else if (actual == ')') {
            while (!pilaVacia(p) && *(char*)peekPila(p) != '(') {
                char* sacado = (char*)popDato(&p);
                postfix[j++] = *sacado;
                free(sacado);
            }
            if (!pilaVacia(p)) free(popDato(&p));
        } else if (actual == '+' || actual == '-' || actual == '*' || actual == '/' || actual == '^') {
            while (!pilaVacia(p) && *(char*)peekPila(p) != '(' && prioridadOp(*(char*)peekPila(p)) >= prioridadOp(actual)) {
                char* sacado = (char*)popDato(&p);
                postfix[j++] = *sacado;
                free(sacado);
            }
            char* item = (char*)malloc(sizeof(char));
            *item = actual;
            pushDato(&p, item);
        }
    }
    while (!pilaVacia(p)) {
        char* sacado = (char*)popDato(&p);
        postfix[j++] = *sacado;
        free(sacado);
    }
    postfix[j] = '\0';
}

int prioridadOp(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

int esOperando(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}
