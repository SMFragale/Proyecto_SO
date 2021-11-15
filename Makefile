all: procesos_solicitantes.c procesos_receptores.c ; gcc -o ps procesos_solicitantes.c -g -lpthread ; cc -o pr procesos_receptores.c -g -lpthread
clean: ; rm ps ; rm pr