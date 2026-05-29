#include <stdio.h>
#include <stdlib.h>
#include "iostring.h"
#include "pila.h"

#define MAX 100

int palindromo(char *cadena);
int verificarParentesis(char *cadena);

int main(void)
{
	char *cadena = NULL;

	inputCadenaDinamica("\n Captura cadena: ",&cadena,MAX);
	printf(" Es palindromo: %s", palindromo(cadena) ? "SI" : "NO");

	inputCadenaDinamica("\n\n Captura expresion: ",&cadena,MAX);
	printf(" Parentesis correctos : %s", verificarParentesis(cadena) ? "SI" : "NO");

	free(cadena);
	printf("\n\n FIN DE PROGRAMA");
	return 0;
}


int palindromo(char *cadena)
{
	Pila pila = {NULL,0,-1,NULL,NULL};
	int i = 0;
	//Primer while, mete los caracteres en la pila, segundo es comparador

	while(cadena[i]){//Recorre la cadena hasta carácter nulo
	if(cadena[i] != ' ') //Ignora espacios
			push(&pila,&cadena[i]);//Guarda la dirección del carácter en la pila.
		i++;
	}
	i = 0;
	while(cadena[i]){
		if(cadena[i] != ' '){
			char *dato = pop(&pila);//Saca ultimo caracter guardado pila
			if(cadena[i] != *dato)//Compara el carácter actual con el que salió de la pila. Dif, no palindromo
				return 0;
		}
		i++;
	}

	return 1;
}



int verificarParentesis(char *cadena)
{
	Pila pila = {NULL,0,-1,NULL,NULL};
	int i = 0;
	char *dato;

	while(cadena[i]){//hasta caracter nulo
		if(cadena[i]=='(' || cadena[i]=='{' || cadena[i]=='[')//si cadena es x valor
			push(&pila,&cadena[i]);//Guarda direccion carac. a la pila
		else if(cadena[i]==')' || cadena[i]=='}' || cadena[i]==']'){
			if(empty(&pila))
				return 0;
			dato=pop(&pila);//saca de la cima

			if( (cadena[i]==')' && *dato!='(') ||
				(cadena[i]=='}' && *dato!='{') ||
				(cadena[i]==']' && *dato!='[') )
				return 0;
		}

		i++;
	}

	if(!empty(&pila))//Si la pila esta vacia, false
		return 0;

	return 1;
}
