#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
/*
Este archivo contiene funciones y constantes que tanto el receptor como los procesos solicitantes requieren
*/

//Operaciones:
const char DEVOLVER_LIBRO[1] = "D";
const char RENOVAR_LIBRO[1] = "R";
const char SOLICITAR_PRESTAMO[1] = "P";

struct Ejemplar {
  char* nombre;
  char status;
  char* fecha;
};

struct Libro {
  char status;
  char* nombre;
  int ISBN;
  struct Ejemplar* ejemplares;
  int numEjemplares;
};



//Este struct representa una solicitud para una operación.
struct Solicitud {
    char operacion;
    char nombre_libro[300];
    char ISBN[10];
    char pipeProceso[20];
};

/*Intenta crear un FIFO con la operación pasada como parámetro*/
/*Si el fifo ya está creado, la operación es ignorada a menos que se presente un error*/
int crearFIFO(const char* operacion) {
    if(mkfifo(operacion, 0777) == -1) {
        if(errno != EEXIST) {
            printf("No se pudo crear el archivo FIFO\n");
            return 1;
        }
    }
}