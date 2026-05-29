#include <stdio.h>
#include "../iostring/iostring.h"
#include "../structures/listadoble/listadoble.h"
#include "Alumno.h"

int main()
{
    ListaD lista = inicializarListaD();
    lista.comparar = compararMatricula;  // default
    lista.imprimir = imprimirAlumno;
    lista.liberar = liberarAlumno;

    int opcion = 0;

    while(opcion != 6)
    {
        printf("\n1. Registrar Alumno");
        printf("\n2. Desplegar Alumnos");
        printf("\n3. Reordenar");
        printf("\n4. Buscar Alumno");
        printf("\n5. Borrar Alumno");
        printf("\n6. Salir");
        printf("\nOpcion: ");
        scanf("%d",&opcion);

        switch(opcion)
        {
            case 1:
            {
                Alumno *nuevo = crearAlumno();

                // Validar matrícula única
                Alumno temp;
                temp.matricula = nuevo->matricula;

                if(buscarDatoD(lista,&temp,compararMatricula) == NULL)
                {
                    insertarOrdenadoD(&lista,nuevo);
                    printf("\nAlumno registrado correctamente\n");
                }
                else
                {
                    printf("\nMatricula repetida\n");
                    free(nuevo);
                }

                break;
            }

            case 2:
            {
                printf("\n--- ASCENDENTE ---\n");
                mostrarListaD(lista);

                printf("\n--- DESCENDENTE ---\n");
                NodoD *aux = lista.fin;
                while(aux)
                {
                    lista.imprimir(aux->dato);
                    aux = aux->ant;
                }
                break;
            }

            case 3:
            {
                int criterio;

                printf("\nReordenar por:");
                printf("\n1. Matricula");
                printf("\n2. Nombre");
                printf("\n3. Semestre");
                printf("\n4. Promedio");
                printf("\nOpcion: ");
                scanf("%d",&criterio);

                switch(criterio)
                {
                    case 1:
                        reordenarD(&lista,compararMatricula);
                        break;
                    case 2:
                        reordenarD(&lista,compararNombre);
                        break;
                    case 3:
                        reordenarD(&lista,compararSemestre);
                        break;
                    case 4:
                        reordenarD(&lista,compararPromedio);
                        break;
                    default:
                        printf("\nOpcion invalida\n");
                }

                break;
            }

            case 4:
            {
                Alumno temp;
                int criterio;

                printf("\nBuscar por:");
                printf("\n1. Matricula");
                printf("\n2. Nombre");
                printf("\n3. Semestre");
                printf("\n4. Promedio");
                printf("\nOpcion: ");
                scanf("%d",&criterio);

                switch(criterio)
                {
                    case 1:
                        printf("Matricula: ");
                        scanf("%u",&temp.matricula);
                        lista.imprimir(
                            buscarDatoD(lista,&temp,compararMatricula)
                        );
                        break;

                    case 2:
                        printf("Nombre: ");
                        scanf(" %[^\n]",temp.nombre);
                        lista.imprimir(
                            buscarDatoD(lista,&temp,compararNombre)
                        );
                        break;

                    case 3:
                        printf("Semestre: ");
                        scanf("%d",&temp.semestre);
                        lista.imprimir(
                            buscarDatoD(lista,&temp,compararSemestre)
                        );
                        break;

                    case 4:
                        printf("Promedio: ");
                        scanf("%f",&temp.promedio);
                        lista.imprimir(
                            buscarDatoD(lista,&temp,compararPromedio)
                        );
                        break;

                    default:
                        printf("\nOpcion invalida\n");
                }

                break;
            }

            case 5:
            {
                Alumno temp;

                printf("\nEliminar por matricula\n");
                printf("Matricula: ");
                scanf("%u",&temp.matricula);

                borrarDatoD(&lista,&temp,compararMatricula);
                printf("\nRealizado\n");

                break;
            }

            case 6:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpcion invalida\n");
        }
    }

    borrarListaD(&lista);
    return 0;
}
