
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
    if(argc == 5 && argv[1] == "-i" && argv[3] == "-p") { //Significa que hay un archivo solicitante
        char* nombreArchivo = argv[2];
        nombrePipe = argv[4];
    }
    else if(argc == 3 && argv[1] == "-p") { //Se requiere el menú pues no hay archivo
        nombrePipe = argv[2];
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
void generarMenu() {

}