struct Ejemplar {
  char* nombre;
  char status;
  char* fecha;
};

struct Libro {
  char status;
  char* nombre;
  int ISBN;
  Ejemplar* ejemplares;
  int numEjemplares;
};
