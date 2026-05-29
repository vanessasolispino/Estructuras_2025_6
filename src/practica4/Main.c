#include <stdio.h>
#include <stdlib.h>
#include <pila.h>
#include <iostring.h>
#include "Laberinto.h"

int main(void)
{
    Laberinto lab = crear_laberinto();
	Coordenada *punto;
	Coordenada origen = {1,1};
	Coordenada destino = {25,24};
	unsigned int nuevasCoor = 0;
	inputEnteroSinSigno("\n Nuevas coordenadas? (1:SI 0:NO) : ",&nuevasCoor);
	if(nuevasCoor)
	{
		inputEntero("\n Captura Origen X: ",&origen.x);
		inputEntero(" Captura Origen Y: ",&origen.y);
		inputEntero("\n Captura Destino X: ",&destino.x);
		inputEntero(" Captura Destino Y: ",&destino.y);
	}
	setOrigen(lab,origen);
	setDestino(lab,destino);
	imprimirLab(lab);
	Pila pila = {NULL,0,-1,imprimirCoordenada,free};
	//PUSH INICIAL DEL ORIGEN
	punto = crearCoordenada(origen.x,origen.y);
	pushDato(&pila,punto);

//CICLO PRINCIPAL
	while( !pilaVacia(pila) )
	{
		punto = peekPila(pila); // donde estamos

		// 1. Condición de salida: se llego al destino?
		if(punto->x == destino.x && punto->y == destino.y) {
			break; // Fin laberinto
		}

		// 2. Dejar "migaja" si es un camino normal (no sobreescribir 'A')
		if(lab[punto->x][punto->y] != 'A' && lab[punto->x][punto->y] != 'B') {
			lab[punto->x][punto->y] = '.'; // Marcamos como camino recorrido
		}

		// 3. Revisar hacia dónde podemos ir
		Movimientos mov = movimientosPosibles(lab, *punto);

		// 4. Avanzar según la prioridad solicitada
		if(mov.right) {
			pushDato(&pila, crearCoordenada(punto->x, punto->y + 1));
		}
		else if(mov.left) {
			pushDato(&pila, crearCoordenada(punto->x, punto->y - 1));
		}
		else if(mov.down) {
			pushDato(&pila, crearCoordenada(punto->x + 1, punto->y));
		}
		else if(mov.up) {
			pushDato(&pila, crearCoordenada(punto->x - 1, punto->y));
		}
		else {
			// 5. REGRESAR (Backtracking): No hay movimientos
			// Marcamos como camino cerrado, no hay salida
			if(lab[punto->x][punto->y] != 'A') {
				lab[punto->x][punto->y] = 'o';
			}
			free( popDato(&pila) ); // Retrocede
		}
	}
	//CICLO
	/*
	while( !pilaVacia(pila))
	{
		punto = peekPila(pila);
		mov = movimientosPosibles(lab,*punto);
		//REGRESAR

		if(bloqueado(mov))
		{
			free( popDato(&pila) );
		}

		//MOVERTE
	}
	*/

// === IMPRESIÓN DEL CAMINO ===
	printf("\n\nCamino encontrado (Secuencia de coordenadas de A hacia B):\n");

	// Crear una pila temporal para invertir el orden del recorrido
	Pila pilaInvertida = {NULL, 0, -1, imprimirCoordenada, free};

	while(!pilaVacia(pila)) {
		// Sacamos original y metemos en invertida
		pushDato(&pilaInvertida, popDato(&pila));
	}

	// Vaciar pila invertida, imprimir coordenadas y liberar memoria
	while(!pilaVacia(pilaInvertida)) {
		punto = (Coordenada*) popDato(&pilaInvertida);
		imprimirCoordenada(punto);

		if(!pilaVacia(pilaInvertida)) {
			printf(" -> ");
		}

		free(punto); // Liberar memoria de cada coordenada
	}
	printf("\n\n");

	// Imprimir lab final
	printf("Vista del laberinto recorrido:\n");
	imprimirLab(lab);
	liberarLaberinto(lab);
    printf("\n\n FIN DE PROGRAMA\n\n");
    return 0;
}


