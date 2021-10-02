
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void generarMenu(char* nombrePipe);
void leerProcesos(char* nombrePipe, char* nombreArchivo);

/*Aquí debería recibir los comandos de la terminal*/

/*
Los procesos solicitantes se invocarán desde uno o varios terminales de la siguiente forma: 
$ solicitante [–i file] –p pipeReceptor 
Donde: 
• Lo que va entre corchetes es opcional. 
• El flag –i solo se indica si la entrada del proceso vendrá de un archivo. A continuación 
del flag se escribe el nombre del archivo.  
• -p: esté flag indica que a continuación viene el nombre del pipe nominal que permitirá 
la comunicación entre procesos.   
• pipeReceptor:  es  el  nombre  del  pipe  que  servirá  de  comunicación  entre  el  proceso 
solicitante  PS y el proceso Receptor. Todos los PS enviarán sus solicitudes por este pipe, 
tal y como se muestra en la figura 2.
*/

/*Los argumentos son los siguientes: 
$ solicitante [–i file] –p pipeReceptor 
Donde: 
• Lo que va entre corchetes es opcional. 
• El flag –i solo se indica si la entrada del proceso vendrá de un archivo. A continuación 
del flag se escribe el nombre del archivo.  
• -p: esté flag indica que a continuación viene el nombre del pipe nominal que permitirá 
la comunicación entre procesos.   
• pipeReceptor:  es  el  nombre  del  pipe  que  servirá  de  comunicación  entre  el  proceso 
solicitante  PS y el proceso Receptor. Todos los PS enviarán sus solicitudes por este pipe, 
tal y como se muestra en la figura 2.*/
int main(int argc, char *argv[]) {
    char* nombrePipe;
    if(argc == 5) { //Significa que hay un archivo solicitante
        char* nombreArchivo = argv[2];
        nombrePipe = argv[4];
        leerProcesos(nombrePipe, nombreArchivo);
    }
    else if(argc == 3) { //Se requiere el menú pues no hay archivo
        nombrePipe = argv[2];
        generarMenu(nombrePipe);
    }
    else {
        printf("Error en los argumentos\n");
    }
    //Siguientes operaciones
}


/*
En caso de requerir un menú,
operación  a  realizar, 
nombre del libro e ISBN. Ante la operación solicitada y según lo especificado en la sección anterior, 
el usuario recibirá una respuesta. El menú le dará al usuario la opción de continuar introduciendo 
operaciones o de salir. Si el usuario decide salir se termina el programa. 
*/
void generarMenu(char* nombrePipe) {
    bool terminar = false;
    while(!terminar) {
        printf("Menu Principal\n");
        printf("1. Devolver un libro\n");
        printf("2. Renovar un libro\n");
        printf("3. Solicitar préstamo de un libro\n");
        printf("4. Salir\n");
        int respuesta;
        scanf("%d", &respuesta);
        if(respuesta == 1) {
            //realizar proceso de devolver un libro
        }
        else if(respuesta == 2) {
            //realizar proceso de renovar un libro
        }
        else if(respuesta == 3) {
            //realizar proceso de solicitar préstamo
        }
        else {
            //Salir
            exit(0);
        }
    }
}

/*Realiza lo mismo que el menú, pero en vez de leer las operaciones desde un menú los lee desde un archivo*/
void leerProcesos(char* nombrePipe, char* nombreArchivo) {
    FILE *entrada = fopen(nombreArchivo, "r"); //Abre el archivo en forma de solo lectura
    if(entrada == NULL) {
        printf("Error, el archivo provisto no existe en la carpeta\n");
        exit(-1);
    }
    char linea[100];
    //Lee linea por linea. Cada linea corresponde a un proceso con la siguiente forma:
    //OPERACION, NOMBRE DEL LIBRO, ISBN
    //EJEMPLO: R, Hamlet, 234

    //Tambíen se podría hacer de forma más sencilla por medio de tokenizer con la coma (,)
    while(fgets(linea, 100, entrada) != NULL) {
        printf("%s", linea);
        //Hacer algo con la línea leída
    }
}