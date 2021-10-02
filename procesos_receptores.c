#include <stdio.h>
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

int main(int argc, char *argv[]) {
    char* pipeReceptor, filedatos;
    if(argc == 7 && argv[1] == "-p" && argv[3] == "-f" && argv[5] == "-s") { //Indica que entran todos los comandos
        pipeReceptor = argv[2];
        filedatos = argv[4];
        char* filesalida = argv[5];
    }
    else if(argc == 5 && argv[1] == "-p" && argv[3] == "-f") { //Comandos sin el archivo de salida
        pipeReceptor = argv[2];
        filedatos = argv[4];
    }
    else {
        printf("Error en los argumentos\n");
    }
}