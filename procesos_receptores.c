#include "intermediario.h"
#define BUFSIZE 512
#define BUFCNT  4
#define N 5

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
void cargarBDInicial(char* archivo);

struct Solicitud buffer[N];

int numLibros;
struct Biblioteca biblioteca;

void* input(void* args) {
    while(1) {
        printf("1. Hola Mundo\n");
        printf("2. Adios mundo cruel\n");
        printf("4. Salir\n");
        int val;
        scanf("%i", &val);
        if(val == 1) {
            printf("Usted ingresó 1\n");
        }
        else if(val == 4) {
            printf("Adiós\n");
            exit(0);
        }
        else {
            printf("Usted no ingresó 1\n");
        }
    }
}

int main(int argc, char *argv[]) {
    int contador = 0, entrada = 0;
    pthread_t id[2];
    pthread_create(&id[0], NULL, input, &buffer);

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

    cargarBDInicial(filedatos);

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
        //Buffer meter sol en el buffer
        printf("Se recibió una solicitud: ");
        while(1){   //Se ejecuta una vez
            while(contador == N){
                buffer[entrada]= sol;   //La variable entra al buffer en el espacio de moemoria "entrada" inicializado en 0
                entrada = (entrada + 1) % N;    //La variable entrada (indice del buffer) aumenta en relación a N. N es 5
                contador = contador + 1;    //La variable contador controla la cantidad de ejecuciones del while  
            }
            printf("%c, ", sol.operacion);
            printf("%s, %s\n", sol.nombre_libro, sol.ISBN);       
            close(fd); 
        }

        generarRespuesta(sol);
        //TODO Realiza la confirmación con la base de datos antes de mandar la respuesta. El resultado de esta confirmación se manda en la respuesta
    }
}

void generarRespuesta(struct Solicitud sol) {
    //Verificar base de datos...
    int fd = open(sol.pipeProceso, O_WRONLY);
    if(fd == -1) {
        printf("Se produjo un error al abrir el archivo FIFO\n");
        exit(-1);
    }
    char respuesta[300];
    int encontrado = 0;
    struct Libro libro;
    if(sol.operacion == 'D') {
        for(int i = 0; i < numLibros && encontrado == 0; i++) {
            struct Libro l = biblioteca.libros[i];
            if(strcmp(l.ISBN, sol.ISBN) == 0) {
                encontrado = 1;
                libro = l;
            }
        }
        if(encontrado == 0) {
            strcpy(respuesta, "El libro con el ISBN dado no se encontró en la base de datos");
        }
        else {
            strcpy(respuesta, "La devolución del libro está en proceso");
        }
    }
    else if(sol.operacion == 'R') {
        for(int i = 0; i < numLibros && encontrado == 0; i++) {
            struct Libro l = biblioteca.libros[i];
            if(strcmp(l.ISBN, sol.ISBN) == 0) {
                encontrado = 1;
                libro = l;
            }
        }
        if(encontrado == 0) {
            strcpy(respuesta, "El libro con el ISBN dado no se encontró en la base de datos");
        }
        else {
            strcpy(respuesta, "La renovación de su libro está en proceso");
        }
    }
    else {
        for(int i = 0; i < numLibros && encontrado == 0; i++) {
            struct Libro l = biblioteca.libros[i];
            if(strcmp(l.ISBN, sol.ISBN) == 0) {
                encontrado = 1;
                libro = l;
            }
        }
        if(encontrado == 0) {
            strcpy(respuesta, "El libro con el ISBN dado no se encontró en la base de datos");
        }
        else {
            encontrado = 0;
            for(int i = 0; i < libro.numEjemplares && encontrado == 0; i++) {
                struct Ejemplar ejemplar = libro.ejemplares[i];
                if(ejemplar.status == 'D') {
                    ejemplar.status = 'P';
                    encontrado = 1;
                }
            }
            if(encontrado == 1) {
                strcpy(respuesta, "El préstamo se logró con éxito");
            }
            else{
                strcpy(respuesta, "No hay ejemplares disponibles de este libro");
            }
        }
    }

    if(write(fd, respuesta, 300) == -1) {
        printf("Hubo un error al mandar la respuesta");
    }

    close(fd);
    unlink(sol.pipeProceso);
}

void cargarBDInicial(char* archivo) {
    FILE *entrada = fopen(archivo, "r"); //Abre el archivo en forma de solo lectura
    if(entrada == NULL) {
        printf("Error, el archivo provisto no existe en la carpeta\n");
        exit(-1);
    }
    char linea[320];
    //Lee linea por linea. Cada linea corresponde a un proceso con la siguiente forma:
    //OPERACION, NOMBRE DEL LIBRO, ISBN
    //EJEMPLO: R, Hamlet, 234

    //Tambíen se podría hacer de forma más sencilla por medio de tokenizer con la coma (,)
    numLibros = 0;  
    while(fgets(linea, 320, entrada) != NULL) {
        struct Libro libro;
        char* token = strtok(linea, ",");
        strcpy(libro.nombre, token);
        token = strtok(NULL, ",");
        token++;
        strcpy(libro.ISBN, token);
        //strcpy(libro.numEjemplares, token);
        token = strtok(NULL, ", \n");
        int num = atoi(token);
        libro.numEjemplares = num;
        libro.ejemplares = malloc(sizeof(struct Ejemplar)*num);
        numLibros++;
        for(int i = 0; i < num; i++) {
            struct Ejemplar ejemplar;
            fgets(linea, 320, entrada);
            token = strtok(linea, ",");
            token = strtok(NULL, ",");
            token++;
            ejemplar.status = *token;
            token = strtok(NULL, ",\n");
            token++;
            strcpy(ejemplar.fecha, token);
            libro.ejemplares[i] = ejemplar;
        }
    }
    biblioteca.libros = malloc(sizeof(struct Libro)*numLibros);
    fclose(entrada);

    entrada = fopen(archivo, "r"); //Abre el archivo en forma de solo lectura
    if(entrada == NULL) {
        printf("Error, el archivo provisto no existe en la carpeta\n");
        exit(-1);
    }
    int pos = 0;
    while(fgets(linea, 320, entrada) != NULL) {
        struct Libro libro;
        char* token = strtok(linea, ",");
        strcpy(libro.nombre, token);
        token = strtok(NULL, ",");
        token++;
        strcpy(libro.ISBN, token);
        //strcpy(libro.numEjemplares, token);
        token = strtok(NULL, ", \n");
        int num = atoi(token);
        libro.numEjemplares = num;
        libro.ejemplares = malloc(sizeof(struct Ejemplar)*num);
        numLibros++;
        for(int i = 0; i < num; i++) {
            struct Ejemplar ejemplar;
            fgets(linea, 320, entrada);
            token = strtok(linea, ",");
            token = strtok(NULL, ",");
            token++;
            ejemplar.status = *token;
            token = strtok(NULL, ",\n");
            token++;
            strcpy(ejemplar.fecha, token);
            libro.ejemplares[i] = ejemplar;
        }
        biblioteca.libros[pos++] = libro;
    }
    fclose(entrada);
}