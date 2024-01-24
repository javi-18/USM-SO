//IMPORTS E INICIO DEL PROGRAMA---------------------------------------------------------------------

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.atomic.AtomicBoolean;

public class TareaSO3 {
    public static void main(String[] args) {

//OBTENCION DE LOS DATOS DEL ARCHIVO----------------------------------------------------------------

        long Inicio = System.currentTimeMillis();
        String Archivo = "sopa_de_letras.txt";
        try {
            FileReader Lector = new FileReader(Archivo);
            BufferedReader Texto = new BufferedReader(Lector);

            Integer Dimension;
            String Palabra;
            String linea;

            Dimension = Integer.valueOf(Texto.readLine());
            Palabra = Texto.readLine();
                
            char[][] Matriz = new char[Dimension][];

            for(int i = 0; i < Dimension; i++){
                linea = Texto.readLine();             
                Matriz[i] = linea.replaceAll(" ", "").toCharArray();
            };
        long Lectura = System.currentTimeMillis();


//NORMAL--------------------------------------------------------------------------------------------

            Normal BusquedaNormal = new Normal(0, 0, Palabra, Dimension, Matriz);
            BusquedaNormal.Busqueda();
            long FinNormal = System.currentTimeMillis();
            System.out.println("Tiempo Busqueda Normal: " + (FinNormal-Inicio-(Lectura-Inicio)) + " milisegundos");

//PROCESOS------------------------------------------------------------------------------------------

            ForkJoinPool pool= new ForkJoinPool();

            AtomicBoolean flagpalabra = new AtomicBoolean(false);
            procesosBusqueda principal= new procesosBusqueda(Matriz, Palabra, 0, Dimension-1, 0, Matriz[0].length-1,flagpalabra);

            pool.execute(principal);
            String resultado= principal.join();

            if(resultado !=null){
                System.out.println(resultado);
            }
            long FinProcesos = System.currentTimeMillis();
            System.out.println("Tiempo Busqueda por Procesos: " + (FinProcesos - Inicio - (FinNormal-Inicio)) + " milisegundos");

//HEBRAS--------------------------------------------------------------------------------------------

            Multithreading Thread1 = new Multithreading(0, 0, Palabra, Dimension, Matriz);
            Thread1.start();

            try {
                Thread1.join();
                long FinHebras = System.currentTimeMillis();
                System.out.println("Tiempo Hebras: " + (FinHebras - Inicio - (FinProcesos - Inicio)) + " milisegundos");
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

//FIN DEL PROGRAMA PRINCIPAL------------------------------------------------------------------------

            Texto.close();
        } catch (IOException exception) {
            System.err.println("Error al leer el archivo: " + exception.getMessage());
        };
    };
};