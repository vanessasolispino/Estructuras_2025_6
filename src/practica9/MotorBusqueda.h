
#define MOTOR_BUSQUEDA_H

#include <HashQ.h>
#include "Libro.h"

int foldingISBN(void *data);
int foldingTitulo(void *data);
int foldingAutor(void *data);
int foldingAnio(void *data);
void imprimirElemento(void *data);

#endif
