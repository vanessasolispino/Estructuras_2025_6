#include <stdio.h>
#include "Alumno.h"
#include "Lista.h"

int main()
{
    Lista lista = {NULL,NULL,0,imprimirAlumno,compararNombre,free};
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
                int (*aux)(void*,void*);
                aux = lista.comparar;

                lista.comparar = compararMatricula;
                Resultado res = buscar(lista,nuevo);

                if(res.enco == NULL)
                {
                    lista.comparar = aux;
                    agregarEnOrden(&lista,nuevo);
                    printf("\nAlumno registrado correctamente\n");
                }
                else
                {
                    printf("\nMatricula repetida\n");
                    free(nuevo);
                    lista.comparar = aux;
                }

                break;
            }

            case 2:
            {
                imprimirLista(lista);
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
                        reordenar(&lista,compararMatricula);
                        break;
                    case 2:
                        reordenar(&lista,compararNombre);
                        break;
                    case 3:
                        reordenar(&lista,compararSemestre);
                        break;
                    case 4:
                        reordenar(&lista,compararPromedio);
                        break;
                    default:
                        printf("\nOpcion invalida\n");
                }

                break;
            }

            case 4:{
                int criterio;
                printf("\nSelecciona criterio:");
                printf("\n1. Matricula");
                printf("\n2. Nombre");
                printf("\n3. Semestre");
                printf("\n4. Promedio");
                printf("\nOpcion: ");
                scanf("%d",&criterio);

                Alumno temp;
                Resultado res;
                int (*aux)(void*,void*);
                aux = lista.comparar;

                switch (criterio){

                case 1:
                    printf("\nInserta matricula");
                    scanf("%u",&temp.matricula);
                    lista.comparar = compararMatricula;
                    break;
                    //se recopila en temp la matricula, lista.comparar toma el valor de compararmatricula
                case 2:
                    printf("\nInserta nombre");
                    scanf("%u",&temp.nombre);
                    lista.comparar= compararNombre;
                    break;
                case 3:
                    printf("\nInserta semestre");
                    scanf("%u",&temp.semestre);
                    lista.comparar= compararSemestre;
                    break;
                case 4:
                    printf("\nInserta promedio ");
                    scanf("%u",&temp.promedio);
                    lista.comparar= compararPromedio;
                    break;
                default:
                    printf("\nOpcion invalida\n");
                }
                    res = buscar(lista,&temp);//Respuesta toma el valor de lo encontrado en lista, &temp
                    imprimirResultado(lista,res); //Se manda llamar la funcion imprimir
                    lista.comparar = aux;
                    break;

            }

                case 5:
        {
                    Alumno temp;

                    printf("\nElimina por matricula\n");
                    printf("Matricula: ");
                    scanf("%u",&temp.matricula);

                    int (*aux)(void*,void*);
                    aux = lista.comparar;

                    lista.comparar = compararMatricula;

                    eliminarDato(&lista,&temp);

                    lista.comparar = aux;

                    printf("\nRealizado\n");
                    break;
                    }

            case 6:
            {
                printf("\nFinalizando programa...\n");
                break;
            }

            default:
                printf("\nOpcion invalida\n");
        }

    }

    return 0;
}
