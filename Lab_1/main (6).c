//LIBRERIAS NECESARIAS-------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

//VARIABLES NECESARIAS-------------------------------------------------------

char linea[410];
char palabra[210];
char orientacion[11];
char linealimpia[210];
char matriz[210][210];
struct dirent* entrada;

//FUNCION MOVER ARCHIVO------------------------------------------------------
//     Esta funcion lo que hace es mover los archivos mediante el nombre,
//     la orientacion y el tamaño que se especifiquen segun el algoritmo de
//     busqueda empleado por el programa.
//
//     PARAMETROS:
//          const char *archivo = nombre del archivo que sera movido.
//          const char *orientacion = orientacion de la sopa de letras del archivo.
//          int tamano_sopa = tamaño de la sopa de letras del archivo.
//---------------------------------------------------------------------------

void moveFile(const char *archivo, const char *orientacion, int tamano_sopa) {
    const char *tamano;
    if (tamano_sopa <= 51) {
        tamano = "50x50";
    } else if (tamano_sopa >= 200) {
        tamano = "200x200";
    } else {
        tamano = "100x100";
    }

    const char *carpeta_a_mover = strstr(orientacion, "vertical") ? "Vertical" : "Horizontal";

    char origen[100];
    snprintf(origen, sizeof(origen), "%s", archivo);

    char destino[100];
    snprintf(destino, sizeof(destino), "%s/%s/%s", carpeta_a_mover, tamano, archivo);

    if (rename(origen, destino) != 0) {
        perror("Error al mover el archivo");
    } else {
        printf("Archivo movido exitosamente.\n");
    }
}

//FUNCION PRINCIPAL DEL PORGRAMA---------------------------------------------

int main() {
    DIR *directorio = opendir(".");
    entrada = readdir(directorio);
    if (directorio == NULL) {
        printf("ERROR DE DIRECTORIO\n");
        return 1;
    }

//CREAR LOS DIRECTORIOS------------------------------------------------------

    const char*Carpetas_principales[]={"Horizontal","Vertical"};
    const char*Subcarpetas[]={"50x50","100x100","200x200"};
    int num_princip= sizeof(Carpetas_principales)/sizeof(Carpetas_principales[0]);
    int num_subcarp= sizeof(Subcarpetas)/sizeof(Subcarpetas[0]);
    int permisos= S_IRWXU | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH;


    for(int i=0; i<num_princip;i++){
        int resultante=mkdir(Carpetas_principales[i],permisos);
        if(resultante==0){
            printf("Carpeta principal %s creada exitosamente.\n", Carpetas_principales[i]);

            for(int j = 0;j < num_subcarp; j++){
                char ruta_subcarp[100];
                snprintf(ruta_subcarp, sizeof(ruta_subcarp), "%s/%s", Carpetas_principales[i], Subcarpetas[j]);
                resultante=mkdir(ruta_subcarp,permisos);
                if(resultante==0){
                    printf("Subcarpeta %s creada exitosamente.\n", Subcarpetas[j]);
                }else{
                    perror("Error al crear la subcarpeta");
                }
            }
        }else{
            perror("Error al crear la carpeta principal");
        }
    }

//CICLO PARA FILTRAR LOS TXT Y OPERAR EN ELLOS-------------------------------

    while (entrada != NULL){
        if (strstr(entrada->d_name,".txt")){
            FILE *archivo = fopen(entrada->d_name, "r");
            if (archivo == NULL) {
                printf("ERROR DE ARCHIVO\n");
                return 1;
            }
            printf("%s\n",entrada->d_name);
            clock_t inicio = clock();
            
//GUARDADO DE LA PALABRA A ENCONTRAR EN MAYUSCULAS---------------------------

            memset(palabra, '\0', sizeof(palabra));
            for (int i = 0; entrada->d_name[i] != '.'; ++i){
		        palabra[i] = toupper(entrada->d_name[i]);
	        }

//ORDENADO DE HORIZONTALES---------------------------------------------------

            memset(orientacion, '\0', sizeof(orientacion));
            fgets(orientacion, sizeof(orientacion), archivo);

            if((strstr(orientacion,"horizontal"))||(strstr(orientacion,"Horizontal"))){

                for(int fila = 1;fgets(linea, sizeof(linea), archivo)!= NULL; fila++){
                    memset(linealimpia, '\0', sizeof(linealimpia));
                    for(int i = 0, j = 0; i<strlen(linea); i += 2, j++){  
                        linealimpia[j] = linea[i];
                    }

//ORDENADO DE HORIZONTALES AL ENCONTRAR, POR LARGO Y MOVIMIENTO DE ARCHIVO---

                    if(strstr(linealimpia,palabra)){
                        clock_t fin = clock();
                        printf("Palabra '%s' encontrada en la fila %d.\n", palabra, fila);
                        double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
                        printf("Tiempo transcurrido: %f segundos.\n", tiempo);

                        int largo = strlen(linealimpia);
                        moveFile(entrada->d_name,orientacion,largo);
                        break;
                    }
                }

//ORDENADO DE VERTICALES-----------------------------------------------------

            } else if((strstr(orientacion,"vertical"))||(strstr(orientacion,"Vertical"))){

                for(int fila = 0; fgets(linea, sizeof(linea), archivo)!= NULL; fila++){
                    memset(linealimpia, '\0', sizeof(linealimpia));
                    for(int i = 0, j = 0; i<strlen(linea); i += 2, j++){  
                        linealimpia[j] = linea[i];
                    }
                    strcpy(matriz[fila],linealimpia);
                }

//ORDENADO DE VERTICALES AL ENCONTRAR, POR LARGO Y MOVIMIENTO DE ARCHIVO-----

                int largo = strlen(matriz[0]);
                int filasmax = largo;
                int columnasmax = largo;
                int longitud = strlen(palabra);

                for (int columna = 0; columna < columnasmax; columna++) {
                    for (int fila = 0; fila <= filasmax - longitud; fila++) {
                        int encontrada = 1;
                        for (int i = 0; i < longitud; i++) {
                            if (matriz[fila + i][columna] != palabra[i]) {
                                encontrada = 0;
                                break;
                            }
                        }

                        if (encontrada) {
                            clock_t fin = clock();
                            printf("Palabra '%s' encontrada en la columna %d con la fila %d.\n", palabra, columna, fila);
                            double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
                            printf("Tiempo transcurrido: %f segundos.\n", tiempo);
                            moveFile(entrada->d_name,orientacion,largo);
                        }
                    }
                }
                
//FIN DE CICLOS Y PROGRAMA---------------------------------------------------

            } else {
                printf("ERROR DE ORIENTACION\n");
                return 1;
            }
            fclose(archivo);
        }
        entrada = readdir(directorio);
    }
    closedir(directorio);
    return 0;
}