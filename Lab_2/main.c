//INCLUDES NECESARIOS E INICIO DEL PROGRAMA------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tablero.h"

//FUNCIONES DEL TABLERO--------------------------------------------------------

void cargarparte(Tablero*tablero,const char*nombre_archivo){
    FILE*archivo= fopen(nombre_archivo,"r");
    if(archivo==NULL){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            fscanf(archivo, " %2s", tablero->tablero[fila][columna]);
        }
    }
    fclose(archivo);
}

void imprimirtablero(const Tablero *tablero){
     for (int fila = 0; fila < FILAS; fila++) {
        for (int columna = 0; columna < COLUMNAS; columna++) {
            printf("%s ", tablero->tablero[fila][columna]);
        }
        printf("\n");
    }
}

TableroGrande crearTableroGrande() {
    TableroGrande tableroGrande;
    tableroGrande.filas = FILAS_GRANDE;
    tableroGrande.columnas = COLUMNAS_GRANDE;
    
    for (int i = 0; i < FILAS_GRANDE; i++) {
        for (int j = 0; j < COLUMNAS_GRANDE; j++) {
            tableroGrande.tablero[i][j][0] = ' ';
            tableroGrande.tablero[i][j][1] = ' ';
            tableroGrande.tablero[i][j][2] = '\0';
        }
    }
    
    return tableroGrande;
}

void copiarEnGrande(TableroGrande *tableroGrande, Tablero *tableroPequeno, int filaInicio, int columnaInicio) {
   for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            // Obtener el valor de la celda en el tablero pequeño
            char *celdaPequena = tableroPequeno->tablero[i][j];

            // Copiar el contenido de la celda en el tablero grande
            strncpy(tableroGrande->tablero[filaInicio + i][columnaInicio + j], celdaPequena, 2);
            tableroGrande->tablero[filaInicio + i][columnaInicio + j][2] = '\0';
        }
    }
}

void imprimirTableroGrande(const TableroGrande*tablero) {
    for (int i = 0; i < tablero->filas; i++) {
        for (int j = 0; j < tablero->columnas; j++) {
            printf("%s ", tablero->tablero[i][j]);
        }
        printf("\n");
    }
}

//CREACION DE LAS PIPES--------------------------------------------------------

int main(){

    int pipes[7][2];//[P-1,P-2,P-3,P-4]

    for (int i = 0; i < 7; i++){
        if (pipe(pipes[i]) < 0){
            printf("Error al crear la tuberia %d",i);
            exit(-1);
        };
    };

//CREACION DE LOS JUGADORES/PROCESOS HIJOS-------------------------------------

    pid_t pidHijo; 
    int TotalProcesos = 5;
    pid_t pids[TotalProcesos];
    pids[0] = getpid();//[Padre,Jugador1,Jugador2,Jugador3,Jugador4]

    for (int i = 1; i < TotalProcesos; i++){
        pidHijo = fork();

        if (pidHijo < 0){
            printf("Error al crear el proceso %d",i);
            exit(1);

        } else if (pidHijo == 0){  
            pids[i] = getpid();
            break;

        } else {
            pids[i] = pidHijo;
        };
    };

//TRASPASO DEL IDENTIFICADOR DE CADA PROCESO-----------------------------------
    
    if (getpid() == pids[0]){
        for (int i = 1; i < TotalProcesos; i++){
            write(pipes[i-1][1], &pids, sizeof(pids));
        };       
    } else {
        for (int i = 1; i < TotalProcesos; i++){
            if (getpid() == pids[i]){
                read(pipes[i-1][0], &pids, sizeof(pids));              
            };
        };
    };

//GENERADO DEL MAZO DE ACCIONES, DESORDEN Y REPARTO DE ESTE--------------------

    char AccionAsignada[9];
   
    if (getpid() == pids[0]){
        srand(time(NULL));
        char MazoCartas[4][9] = {
            "Buscar","Buscar","Escalera","Escalera"};

        for (int i = 0; i < 4; i++) {
            int j = rand() % 4;
            memcpy(AccionAsignada,MazoCartas[i],9);
            memcpy(MazoCartas[i],MazoCartas[j],9);
            memcpy(MazoCartas[j],AccionAsignada,9);
        };
        for (int i = 1; i < TotalProcesos; i++){
            memcpy(AccionAsignada,MazoCartas[i-1],9);
            write(pipes[i-1][1], &AccionAsignada, sizeof(AccionAsignada)); 
        };

    } else {
        for (int i = 1; i < TotalProcesos; i++){
            if (getpid() == pids[i]){
                read(pipes[i-1][0], &AccionAsignada, sizeof(AccionAsignada));
            };
        };
    }

//GENERADO DEL MAZO DE TABLEROS Y DESORDEN DE ESTE-----------------------------

    if(getpid() == pids[0]){
       Tablero tableros[9];
        const char *nombresArchivos[9] = {
            "Inicio.txt",
            "tablero1.txt",
            "tablero2.txt",
            "tablero3.txt",
            "tablero4.txt",
            "tablero5.txt",
            "tablero6.txt",
            "tablero7.txt",
            "tablero8.txt"
        };

        for (int i = 0; i < 9; i++){
            cargarparte(&tableros[i], nombresArchivos[i]);
        }  
        
        TableroGrande tableroGrande = crearTableroGrande();
        for(int i = 0; i < FILAS_GRANDE; i++) {
            for(int j = 0; j < COLUMNAS_GRANDE; j++) {
                tableroGrande.tablero[i][j][0] = 'X'; 
                tableroGrande.tablero[i][j][1] = ' ';
            }
        }

        int filainicio=6;
        int columnainicio=6;
        copiarEnGrande(&tableroGrande, &tableros[0], filainicio, columnainicio);
        write(pipes[0][1], &tableroGrande, sizeof(tableroGrande));
    };

//CICLO DE INICIO TURNOS DEL JUEGO---------------------------------------------

    if (getpid() == pids[1]){
        TableroGrande tableroGrande;
        read(pipes[0][0], &tableroGrande, sizeof(tableroGrande));

        int Continuar = 1;
        bool Victoria = false;
        int RondasRestantes = 15;
        int RondaActual = 1;
        int Tesoros = 0;
        int Accion;
        while((Continuar)&&(RondasRestantes >= RondaActual)){

//CICLO DE MENU DEL JUEGO POR JUGADOR-------------------------------------------

            for(int i = 1; i < TotalProcesos; i++){
                printf("\n-----------------------------------\n");
                printf("Tablero Inicial:\n");
                imprimirTableroGrande(&tableroGrande);

                printf("-----------------------------------\n");
                printf("|        Turno del Jugador %d      |\n",i);

                char AccionJugador[9];

                if(i > 1){
                    close(pipes[i+2][1]);
                    read(pipes[i+2][0], &AccionJugador, sizeof(AccionJugador));
                };

                bool PasarAccion = true;
                while(PasarAccion){
                    Accion = 0;

//RONDA ACTUAL Y RONDAS TOTALES------------------------------------------------

                    printf("-----------------------------------\n");
                    if(RondaActual<10){
                        printf("|            Ronda %d/%d           |", RondaActual,RondasRestantes);
                    } else {
                        printf("|            Ronda %d/%d          |", RondaActual,RondasRestantes);
                    };

//MOSTRAR MAPA-----------------------------------------------------------------



//MOSTRAR ACCIONES A REALIZAR Y DESICIONES-------------------------------------

                    printf("\n-----------------------------------\n");
                    printf("|        Que deseas hacer?        |\n");
                    printf("         1. Mover Arriba       \n");
                    printf("         2. Mover Izquierda        \n");
                    printf("         3. Mover Dererecha        \n");
                    printf("         4. Mover Abajo        \n");
                    if(i == 1){
                        printf("         5. Acción %s      \n",AccionAsignada);
                    } else {
                        printf("         5. Acción %s      \n",AccionJugador);
                    }
                    printf("         6. SPAWN TESORO(SKIP)        \n");
                    printf("| Ingresa el Numero de la Accion  |\n");
                    scanf("%d", &Accion);
                    printf("-----------------------------------\n\n\n\n\n\n");

//ACCIONES POSIBLES-----------------------------------------------------------

                    if(Accion == 1){
                        printf("-----------------------------------\n");
                        printf("|       ARRIBA (NO FUNCIONA)      |");
                        PasarAccion = false;

                    } else if(Accion == 2){
                        printf("-----------------------------------\n");
                        printf("|     IZQUIERDA (NO FUNCIONA)     |");
                        PasarAccion = false;

                    } else if(Accion == 3){
                        printf("-----------------------------------\n");
                        printf("|      DERECHA (NO FUNCIONA)      |");
                        PasarAccion = false;

                    } else if(Accion == 4){
                        printf("-----------------------------------\n");
                        printf("|       ABAJO (NO FUNCIONA)       |");
                        PasarAccion = false;

                    } else if(Accion == 5){
                        if(i == 1){
                            printf("-----------------------------------\n");
                            printf("   ACCION %s (NO FUNCIONA)",AccionAsignada);
                        } else {
                            printf("-----------------------------------\n");
                            printf("   ACCION %s (NO FUNCIONA)",AccionJugador);
                        }
                        PasarAccion = false;

                    } else if(Accion == 6){
                        printf("-----------------------------------\n");
                        printf("|  EL JUGADOR %d OBTUVO UN TESORO  |",i);
                        PasarAccion = false;
                        Tesoros++;

                        if(Tesoros >= 4){
                            Continuar = 0;
                            Victoria = true;
                            break;          
                        };

                    } else {
                        printf("-----------------------------------\n");
                        printf("|        Error de Seleccion       |\n");
                        printf("|  Ingresa el Numero Nuevamente   |\n");
                    };
                };
            };

//COMPROBAR TESOROS RECOGIDOS Y TURNOS PARA PROCESOS HERMANOS------------------

            RondaActual++;
            if(RondaActual > RondasRestantes){
                Continuar = 0;
            };
            close(pipes[4][0]);
            close(pipes[5][0]);
            close(pipes[6][0]);
            write(pipes[4][1], &Continuar, sizeof(Continuar));
            write(pipes[5][1], &Continuar, sizeof(Continuar));
            write(pipes[6][1], &Continuar, sizeof(Continuar));
        };

//FIN DEL JUEGO POR TURNOS O DERROTA-------------------------------------------

        if(Victoria == false){   
            printf("\n-----------------------------------\n");
            printf("|          FIN DEL JUEGO          |\n");
            printf("|    NO LOGRARON ESCAPAR TODOS    |");
            printf("\n-----------------------------------\n");
        };

//FIN DEL JUEGO POR VICTORIA---------------------------------------------------

        if(Victoria == true){  
            printf("\n-----------------------------------\n");
            printf("|           FELICIDADES           |\n");
            printf("|     LOGRARON ESCAPAR TODOS      |");
            printf("\n-----------------------------------\n");
        };
    };

//JUGADORES 2,3 Y 4------------------------------------------------------------

    if((getpid()== pids[2])||(getpid()== pids[3])||(getpid()== pids[4])){
        int Continuar = 1;
        while(Continuar){
            if(getpid()== pids[2]){
                printf("LLEGA PROCESO 2.1");
                close(pipes[4][0]);
                printf("LLEGA PROCESO 2");
                write(pipes[4][1], &AccionAsignada, sizeof(AccionAsignada));
                pipe(pipes[4]);
                close(pipes[4][1]);
                read(pipes[4][0], &Continuar, sizeof(Continuar));
                pipe(pipes[4]);
            } else if(getpid()== pids[3]){
                close(pipes[5][0]);
                printf("LLEGA PROCESO 3");
                write(pipes[5][1], &AccionAsignada, sizeof(AccionAsignada));
                pipe(pipes[5]);
                close(pipes[5][1]);
                read(pipes[5][0], &Continuar, sizeof(Continuar));
                pipe(pipes[5]);
            } else if(getpid()== pids[4]){
                close(pipes[6][0]);
                printf("LLEGA PROCESO 4");
                write(pipes[6][1], &AccionAsignada, sizeof(AccionAsignada));
                pipe(pipes[6]);
                close(pipes[6][1]);
                read(pipes[6][0], &Continuar, sizeof(Continuar));
                pipe(pipes[6]);
            };

            if(Continuar == 0){
                break;
            };
        };      
    };  

//CIERRE DE PIPES--------------------------------------------------------------

    for (int i = 0; i < 7; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    };

//ESPERA DEL PADRE A LOS HIJOS Y FIN DE LOS PROCESOS---------------------------

    if(getpid() == pids[0]){
        for (int i = 1; i < TotalProcesos; i++){
            waitpid(pids[i],NULL,0);
        };        
    };
    return 0;
};