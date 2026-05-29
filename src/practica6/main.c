#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

// --- Prototipos de funciones auxiliares ---
int esValida(const char* exp);
void infixAPostfix(const char* infix, char* postfix);
int precedencia(char c);
int esOperando(char c);

int main(void)
{
    int opcion;
    char infix[100];
    char postfix[100];

    do {
        printf("\n=== Practica 6: Infix a Postfix:D\n");
        printf("1. Infix a Postfix\n");
        printf("2. Terminar el programa\n");
        printf("Elige una opción: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = 0;
        }
        getchar(); // Limpiar el buffer

        switch(opcion) {
            case 1:
                printf("Captura la expresión Infix");
                fgets(infix, sizeof(infix), stdin);
                infix[strcspn(infix, "\n")] = 0;

                // Validar uso correcto de paréntesis
                if (esValida(infix)) {
                    infixAPostfix(infix, postfix);
                    printf("-> Expresión Postfix resultante: %s\n", postfix);
                } else {
                    printf("-> ERROR: La expresión no es válida revisa los paréntesis).\n");
                }
                break;

            case 2:
                printf("\n\n FIN DE PROGRAMA\n\n");
                break;

            default:
                printf("Opción inválida. Intenta de nuevo.\n");
        }
    } while(opcion != 2);

    return 0;
}

// --- Implementación de Funciones ---

int esValida(const char* exp) {
    Pila p = {NULL, 0, 100, NULL, NULL};

    for (int i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == '(') {
            char* parenthesis = (char*)malloc(sizeof(char));
            *parenthesis = exp[i];
            pushDato(&p, parenthesis);
        } else if (exp[i] == ')') {
            if (pilaVacia(p)) {
                return 0;
            }
            char* sacado = (char*)popDato(&p);
            free(sacado);
        }
    }

    int valida = pilaVacia(p);

    while(!pilaVacia(p)){
         free(popDato(&p));
    }

    return valida;
}

void infixAPostfix(const char* infix, char* postfix) {
    Pila p = {NULL, 0, 100, NULL, NULL};
    int j = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char actual = infix[i];

        if (esOperando(actual)) {
            postfix[j++] = actual;
        }
        else if (actual == '(') {
            char* item = (char*)malloc(sizeof(char));
            *item = actual;
            pushDato(&p, item);
        }
        else if (actual == ')') {
            while (!pilaVacia(p) && *(char*)peekPila(p) != '(') {
                char* sacado = (char*)popDato(&p);
                postfix[j++] = *sacado;
                free(sacado);
            }
            if (!pilaVacia(p) && *(char*)peekPila(p) == '(') {
                free(popDato(&p));
            }
        }
        else if (actual == '+' || actual == '-' || actual == '*' || actual == '/' || actual == '^') {
            while (!pilaVacia(p) && precedencia(*(char*)peekPila(p)) >= precedencia(actual)) {
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

int precedencia(char c) {
    if (c == '^') return 3;
    if (c == '*' || c == '/') return 2;
    if (c == '+' || c == '-') return 1;
    return 0;
}

int esOperando(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}
