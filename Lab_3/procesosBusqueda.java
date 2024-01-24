import java.util.concurrent.RecursiveTask;
//import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.atomic.AtomicBoolean;

public class procesosBusqueda extends RecursiveTask<String>{
    private char sopa[][];
    private String palabraEncontrada;
    private int fila_inicial,fila_final,columna_inicial,columna_final;
    private AtomicBoolean flagpalabra;

    public procesosBusqueda(char sopa[][],String palabraEncontrada,int fila_inicial,int fila_final,int columna_inicial,int columna_final, AtomicBoolean flagpalabra){
        this.sopa=sopa;
        this.palabraEncontrada=palabraEncontrada;
        this.fila_inicial=fila_inicial;
        this.fila_final=fila_final;
        this.columna_inicial=columna_inicial;
        this.columna_final=columna_final;
        this.flagpalabra=flagpalabra;    
    }

    @Override
    protected String compute(){
        if (flagpalabra.get()) {
            return null;  // Terminar el proceso si la palabra ya se encontró en otro lugar.
        }
        if(fila_final-fila_inicial<palabraEncontrada.length()&& columna_final-columna_inicial<palabraEncontrada.length()){
            String res= BUSCAR_HORIZONTAL(fila_inicial,columna_inicial,fila_final,columna_final);
            if(res !=null){
                return res;
            }
            res=BUSCAR_VERTICAL(fila_inicial,columna_inicial,fila_final,columna_final);
            if(res !=null){
                return res;
            }
            return null;
            
        }
        else{
            int filaM=(fila_inicial+fila_final)/2;
            int columnaM=(columna_inicial+columna_final)/2;

            procesosBusqueda[]subprocesos={
                new procesosBusqueda(sopa, palabraEncontrada, fila_inicial, filaM, columna_inicial, columnaM,flagpalabra),
                new procesosBusqueda(sopa, palabraEncontrada, fila_inicial, filaM, columnaM +1, columna_final,flagpalabra),
                new procesosBusqueda(sopa, palabraEncontrada, filaM +1, fila_final, columna_inicial, columnaM,flagpalabra),
                new procesosBusqueda(sopa, palabraEncontrada, filaM +1, fila_final, columnaM +1, columna_final,flagpalabra),
                
            };


            for(procesosBusqueda subproce:subprocesos){
                String resultante= subproce.invoke();
                if(resultante !=null){
                    return resultante;
                }
            }
            

            for(procesosBusqueda subproce:subprocesos){
                String resultante= subproce.join();
                if(resultante!=null){
                    flagpalabra.set(true);
                    System.out.println("Palabra encontrada:" +resultante);
                    return resultante;
                }
            }

            if(!flagpalabra.get()){
            }
            return null;
        }
    }

    private String BUSCAR_HORIZONTAL(int fila_inicial, int columna_inicial, int fila_final, int columna_final){
        for(int i=fila_inicial;i<=fila_final;i++){
            for(int j=columna_inicial;j<=columna_final -palabraEncontrada.length() +1;j++){
                boolean encontrada=true;
                for(int k=0; k<palabraEncontrada.length();k++){
                    if(sopa[i][j+k]!=palabraEncontrada.charAt(k)){
                        encontrada=false;
                        break;
                    }
                }
                if(encontrada){
                    flagpalabra.set(true);
                    return String.format("Fila %d, Columna %d", (i+1), (j+1));
                }
            }
        }
        return null;
    }
   

    private String BUSCAR_VERTICAL(int fila_inicial,int columna_inicial, int fila_final,int columna_final){
        for(int i=fila_inicial;i<=fila_final - palabraEncontrada.length()+1;i++){
            for(int j = columna_inicial; j <= columna_final; j++){
                boolean encontrada=true;
                for(int k=0;k<palabraEncontrada.length();k++){
                    if(sopa[i+k][j]!=palabraEncontrada.charAt(k)){
                        encontrada=false;
                        break;
                    }
                }
                if(encontrada){
                    flagpalabra.set(true);
                    return String.format("Fila %d, Columna %d", (i+1), (j+1));
                }
            }
        }
        return null;
    }
}