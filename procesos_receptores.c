#include "intermediario.h"

/*
$ ./receptor –p pipeReceptor –f filedatos [ –s filesalida ] 
Donde: 
• -p: esté flag indica que a continuación viene el nombre del pipe nominal que permitirá 
la comunicación entre los procesos.   
• pipeReceptor: es el nombre del pipe que servirá de comunicación entre los procesos 
solicitantes y el proceso receptor. Todos los PS enviarán sus solicitudes por este pipe, 
tal y como se muestra en la figura 2 
• -f indica que el siguiente parámetro es el nombre de un archivo 
• filedatos:  es  el  archivo  donde  se  encuentra  la  BD  inicial  que  maneja  el  sistema  (la 
cantidad de libros y sus características). Se les proveerá archivos de ejemplo y el archivo 
final para el día de la sustentación.   
• -s: este flag indica que a continuación viene el nombre del archivo de salida.  
• filesalida: cuando el receptor no tenga más peticiones, éste termina y escribe en este 
archivo  como  quedó  la  BD:  cuál  es  el estado  de  cada  libro,  cuantos  ejemplares  están 
disponibles de cada libro, y si están prestados, la fecha de devolución. 
*/

void generarRespuesta(struct Solicitud sol);

int main(int argc, char *argv[]) {
    char* pipeReceptor;
    char* filedatos;
    if(argc == 7) { //Indica que entran todos los comandos
        pipeReceptor = argv[2];
        filedatos = argv[4];
        char* filesalida = argv[5];
    }
    else if(argc == 5) { //Comandos sin el archivo de salida
        pipeReceptor = argv[2];
        filedatos = argv[4];
    }
    else {
        printf("Error en los argumentos\n");
        return -1;
    }
    //Crea los pipes para la comunicación con los procesos
    crearFIFO("./pipes/D");
    crearFIFO("./pipes/R");
    crearFIFO("./pipes/P");

    //Crea el pipe principal para la recepción de procesos
    crearFIFO(pipeReceptor);
    while(true) {
      //inciar proceso de recepción de solicitudes
        int fd = open(pipeReceptor, O_RDONLY);
        if(fd == -1) {
            printf("Error al abrir el FIFO\n");
            return 2;
        }
        struct Solicitud sol;
        read(fd, &sol, sizeof(struct Solicitud));
        printf("Se recibió una solicitud: ");
        printf("%c, ", sol.operacion);
        printf("%s, %s\n", sol.nombre_libro, sol.ISBN);       
        close(fd); 

        generarRespuesta(sol);
        //TODO Realiza la confirmación con la base de datos antes de mandar la respuesta. El resultado de esta confirmación se manda en la respuesta
    }
}

void generarRespuesta(struct Solicitud sol) {
    //Verificar base de datos...
    char pipe[9] = "./pipes/";
    char o = sol.operacion;
    strncat(pipe, &o, 1);
    int fd = open(pipe, O_WRONLY);
    if(fd == -1) {
        printf("Se produjo un error al abrir el archivo FIFO\n");
    }
    
    char respuesta[300] = "Placeholder";
    if(write(fd, respuesta, 300) == -1) {
        printf("Ocurrió un error al leer la respuesta\n");
    }
    close(fd);
}