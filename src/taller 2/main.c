#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
//Primer numero es menor a segundo? TF
int comparar(void *a, void *b){
    int x = *(int*)a;//Convertir a de void a int
    int y = *(int*)b;
    return x< y; //Si x menor que devuelve 1 (verdadero) Si no devuelve 0 (falso)
}

void ordenarPila(Pila *pila, int (*comparar)(void*,void*)){

    Pila auxiliar;
    inicializarPila(&auxiliar);

    void *temp;

    while(!pilaVacia(pila)){

        temp = pop(pila);

        while(!pilaVacia(&auxiliar) &&
              comparar(tope(&auxiliar), temp)){

            push(pila, pop(&auxiliar));
        }

        push(&auxiliar, temp);
    }

    while(!pilaVacia(&auxiliar)){
        push(pila, pop(&auxiliar));
    }
}

void imprimirPila(Pila *p){

    Pila aux;
    inicializarPila(&aux);

    while(!pilaVacia(p)){
        int *num = pop(p);
        printf("%d\n", *num);
        push(&aux, num);
    }

    while(!pilaVacia(&aux)){
        push(p, pop(&aux));
    }
}

int main(){

    Pila pila;
    inicializarPila(&pila);

    int valores[] = {8,5,6,10,2,3,9};

    for(int i=0;i<7;i++){
        int *num = malloc(sizeof(int));
    //Se toma un número del arreglo Se crea memoria para guardarlo Se mete a la pila con push
        *num = valores[i];
        push(&pila, num);
    }

    printf("Pila original:\n");
    imprimirPila(&pila);

    ordenarPila(&pila, comparar);

    printf("\nPila ordenada:\n");
    imprimirPila(&pila);

    return 0;
}
