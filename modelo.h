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
