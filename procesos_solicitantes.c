
#include "intermediario.h"

void generarMenu(char* nombrePipe);
void leerProcesos(char* nombrePipe, char* nombreArchivo);
int procesamiento(char* pipe, char operacion, char* nombreLibro, char* ISBN);
void solicitarRespuesta(char* pipe);

void generarMenu(char* nombrePipe);
void leerProcesos(char* nombrePipe, char* nombreArchivo);
void *tfunc (void *args);

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
    int i;
    //Esto estaba generando errores
    //pthread_t id_hilo[NTHREADS];

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
    //Prueba con hilos
    /* Este código genera errores
    for(int i=0; i<3; i++){
        pthread_create(&id_hilo[i], NULL, &tfunc, NULL); //Crear hilos
    }

    
    for(int i=0; i<3; i++){
        pthread_join(&id_hilo[i], NULL);
    }
    */
    
    
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
        printf("\nMenu Principal\n");
        printf("1. Devolver un libro\n");
        printf("2. Renovar un libro\n");
        printf("3. Solicitar préstamo de un libro\n");
        printf("4. Salir\n");
        int respuesta;
        scanf("%d", &respuesta);
        printf("\n");
        if(respuesta == 1 || respuesta == 2 || respuesta == 3) {
            //realizar un proceso
            int c;
            while((c = getchar()) != '\n' && c != EOF) {}

            char nomLibro[300];
            char isbn[10];
            printf("Ingrese el nombre del libro\n");
            scanf("%[^\n]s", nomLibro);
            getchar();
            printf("Ingrese el ISBN del libro\n");
            scanf("%[^\n]s", isbn);
            getchar();
            char op;
            if(respuesta == 1) {
                op = DEVOLVER_LIBRO[0];
            }
            else if(respuesta == 2) {
                op = RENOVAR_LIBRO[0];
            }
            else {
                op = SOLICITAR_PRESTAMO[0];
            }
            procesamiento(nombrePipe, op, nomLibro, isbn);
        }
        else if(respuesta == 4) {
            exit(0);
        }
        else {
            printf("Entrada inválida\n");
        }
    }
}

//TODO
/*Realiza lo mismo que el menú, pero en vez de leer las operaciones desde un menú los lee desde un archivo*/
void leerProcesos(char* nombrePipe, char* nombreArchivo) {
    FILE *entrada = fopen(nombreArchivo, "r"); //Abre el archivo en forma de solo lectura
    if(entrada == NULL) {
        printf("Error, el archivo provisto no existe en la carpeta\n");
        exit(-1);
    }
    char linea[320];
    //Lee linea por linea. Cada linea corresponde a un proceso con la siguiente forma:
    //OPERACION, NOMBRE DEL LIBRO, ISBN
    //EJEMPLO: R, Hamlet, 234

    //Tambíen se podría hacer de forma más sencilla por medio de tokenizer con la coma (,)
    while(fgets(linea, 320, entrada) != NULL) {
            char* token = strtok(linea, ", ");
            struct Solicitud sol;
            sol.operacion = *token;
            token = strtok(NULL, ",");
            token++;
            strcpy(sol.nombre_libro, token);
            token = strtok(NULL, ", \n");
            strcpy(sol.ISBN, token);
            procesamiento(nombrePipe, sol.operacion, sol.nombre_libro, sol.ISBN);
    }
        //Hacer algo con la línea leída
        //OJO Manda el nombre del libro con un espacio a la izquierda!
}

/*
Los 3 procesos se comunican con el servidor (procesos_receptores)
Por medio de los archivos FIFO que actúan como pipes para mandar
la información. 
*/
int procesamiento(char* pipe, char operacion, char* nombreLibro, char* ISBN) {
    //No va a entrar hasta que el servidor se conecte al FIFO
    int fd = open(pipe, O_WRONLY);
    if(fd == -1) {
        printf("Se produjo un error al abrir el archivo FIFO\n");
        return 1;
    }

    char path[20] = "./pipes/";
    char id[8];
    //Para crear el fifo de comunicación, se utiliza un pipe identificado con el p_id de este proceso
    //Para evitar potenciales problemas, una vez el servidor es apagado, los fifos son eliminados
    pid_t pid = getpid();
    sprintf(id, "%d", pid);
    strcat(path, id);

    //Crea el paquete
    struct Solicitud sol;
    sol.operacion = operacion;
    strcpy(sol.nombre_libro, nombreLibro);
    strcpy(sol.ISBN, ISBN);
    strcpy(sol.pipeProceso, path);

    //Intenta mandar el paquete (la operación) al receptor
    if(write(fd, &sol, sizeof(struct Solicitud)) == -1) {
        printf("Ocurrió un error al mandar la solicitud\n");
        return 2;
    }
    close(fd);
    solicitarRespuesta(path);
}

void solicitarRespuesta(char* path) {
    crearFIFO(path);
    int fd = open(path, O_RDONLY);
    if(fd == -1) {
        printf("Se produjo un error al abrir el archivo FIFO\n");
    }
    char respuesta[300];
    if(read(fd, respuesta, 300) == -1) {
        printf("Ocurrió un error al leer la respuesta\n");
    }
    printf("Respuesta recibida: %s\n", respuesta);
    close(fd);
    sleep(2);
    unlink(path);
}

void *tfunc (void *args) {
    printf("Hello world");
}
