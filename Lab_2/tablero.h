#define FILAS 5
#define COLUMNAS 5
#define FILAS_GRANDE 16
#define COLUMNAS_GRANDE 16
#define MAX_JUGADORES 4 

typedef struct {
    char tablero[FILAS][COLUMNAS][3];
} Tablero;

typedef struct {
    char tablero[FILAS_GRANDE][COLUMNAS_GRANDE][3];
    int filas;
    int columnas;
} TableroGrande;

typedef struct {
    int fila;
    int columna;
    char nombre[2]; // Un arreglo de caracteres con longitud 2
} Jugador;

typedef struct {
    int filas;
    int columnas;
    char direccion;
} Movimiento;

void cargarparte(Tablero*tablero,const char*nombre_archivo);
void imprimirtablero(const Tablero *tablero);
TableroGrande crearTableroGrande();
void imprimirTableroGrande(const TableroGrande*tablero);
void copiarEnGrande(TableroGrande *tableroGrande, Tablero *tableroPequeno, int filaInicio, int columnaInicio);