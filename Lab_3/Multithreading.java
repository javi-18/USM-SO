//PARAMETROS NECESARIOS PARA LA HEBRA---------------------------------------------------------------

public class Multithreading extends Thread{
    
    private Integer VID;
    private Integer HID;
    private String Palabra;
    private char[][] Matriz;
    private Integer Dimension;
    
//FUNCION CONSTRUCTOR DE LA HEBRA-------------------------------------------------------------------

    public Multithreading(Integer VID, Integer HID, String Palabra, Integer Dimension, char[][] Matriz){
        this.VID = VID;
        this.HID = HID;
        this.Matriz = Matriz;
        this.Palabra = Palabra;    
        this.Dimension = Dimension;
    }

//CODIGO QUE EJECUTARAN LAS HEBRAS------------------------------------------------------------------

    @Override
    public void run(){
        if(Matriz[0].length == Palabra.length()){
            BUSCAR_HORIZONTAL(Matriz, Palabra, VID, HID);
            BUSCAR_VERTICAL(Matriz, Palabra, VID, HID);
            Thread.currentThread().interrupt();

        } else {
            Integer NuevaDimension = Dimension/2;
            if(NuevaDimension>=Palabra.length()){
                char[][] SubMatriz1 = new char[NuevaDimension][NuevaDimension];
                char[][] SubMatriz2 = new char[NuevaDimension][NuevaDimension];
                char[][] SubMatriz3 = new char[NuevaDimension][NuevaDimension];
                char[][] SubMatriz4 = new char[NuevaDimension][NuevaDimension];

                for (int i = 0; i < NuevaDimension; i++) {
                    for (int j = 0; j < NuevaDimension; j++) {
                        SubMatriz1[i][j] = Matriz[i][j];
                        SubMatriz2[i][j] = Matriz[i][j + NuevaDimension];
                        SubMatriz3[i][j] = Matriz[i + NuevaDimension][j];
                        SubMatriz4[i][j] = Matriz[i + NuevaDimension][j + NuevaDimension];
                    }
                }

                Multithreading SubThread1 = new Multithreading(VID, HID, Palabra, NuevaDimension, SubMatriz1);
                Multithreading SubThread2 = new Multithreading(VID, HID + NuevaDimension, Palabra, NuevaDimension, SubMatriz2);
                Multithreading SubThread3 = new Multithreading(VID + NuevaDimension, HID, Palabra, NuevaDimension, SubMatriz3);
                Multithreading SubThread4 = new Multithreading(VID + NuevaDimension, HID + NuevaDimension, Palabra, NuevaDimension, SubMatriz4);

                SubThread1.start();
                SubThread2.start();
                SubThread3.start();
                SubThread4.start();
            
                try {
                    SubThread1.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                try {
                    SubThread2.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                try {
                    SubThread3.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                try {
                    SubThread4.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            } else {
                Thread.currentThread().interrupt();
            }
        }
    }

//FUNCION BUSCAR_HORIZONTAL-------------------------------------------------------------------------

    public static void BUSCAR_HORIZONTAL(char[][] Matriz, String Palabra, Integer VID, Integer HID) {

        for(int k = 0; k < Palabra.length(); k++){
            if(Palabra.equals(new String(Matriz[k]))){
                System.out.println("Fila " + (VID + k + 1) + ", Columna " + (HID + 1));
            }               
        }
    }

//FUNCION BUSCAR_VERTICAL---------------------------------------------------------------------------

    public static void BUSCAR_VERTICAL(char[][] Matriz, String Palabra, Integer VID, Integer HID) {

        for(int l = 0; l < Palabra.length(); l++){
            char[] Columna = new char [Palabra.length()];

            for(int m = 0; m < Palabra.length(); m++){
               Columna[m] = Matriz[m][l];
            }
            
            if(Palabra.equals(new String(Columna))){
                System.out.println("Fila " + (VID + l + 1) + ", Columna " + (HID + 1));
            }
        }
    }
}