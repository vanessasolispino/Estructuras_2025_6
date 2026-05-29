#include "Laberinto.h"


Movimientos movimientosPosibles(Laberinto lab, Coordenada punto)
{
	Movimientos movimientos = {0,0,0,0};

	// ARRIBA (Fila anterior, misma columna)
	if( lab[punto.x - 1][punto.y] == '*' || lab[punto.x - 1][punto.y] == 'B')
		movimientos.up = 1;

	// ABAJO (Fila siguiente, misma columna)
	if( lab[punto.x + 1][punto.y] == '*' || lab[punto.x + 1][punto.y] == 'B')
		movimientos.down = 1;

	// DERECHA (Misma fila, columna siguiente)
	if( lab[punto.x][punto.y + 1] == '*' || lab[punto.x][punto.y + 1] == 'B')
		movimientos.right = 1;

	// IZQUIERDA (Misma fila, columna anterior)
	if( lab[punto.x][punto.y - 1] == '*' || lab[punto.x][punto.y - 1] == 'B')
		movimientos.left = 1;

	return movimientos;
}


void imprimirMovimientos(Movimientos movimientos)
{
	printf("\n");
	//ARRIBA
	if( movimientos.up == 1)
		printf(" ARRIBA");
	//ABAJO
	if( movimientos.down == 1)
		printf(" ABAJO");
	//DERECHA
	if( movimientos.right == 1)
		printf(" DERECHA");
	//IZQUIERDA
	if( movimientos.left == 1)
		printf(" IZQUIERDA");

}


int bloqueado(Movimientos mov)
{
	return mov.up == 0 && mov.up == mov.down && mov.up==mov.left && mov.up==mov.right;
}
