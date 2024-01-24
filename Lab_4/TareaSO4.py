#IMPORT NECESARIOS----------------------------------------------------------------------

import threading
import datetime
import random
import time

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO MAT-----------------------------------

semFilaMat = threading.Semaphore(30)
semDepMat = threading.Semaphore(10)
LockMat = threading.Lock()
EsperaMat = False
contMatIn = 0
contMatOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO INF-----------------------------------

semFilaInf = threading.Semaphore(10)
semDepInf = threading.Semaphore(2)
LockInf = threading.Lock()
EsperaInf = False
contInfIn = 0
contInfOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO FIS-----------------------------------

semFilaFis = threading.Semaphore(20)
semDepFis = threading.Semaphore(5)
LockFis = threading.Lock()
EsperaFis = False
contFisIn = 0
contFisOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO QUI-----------------------------------

semFilaQui = threading.Semaphore(9)
semDepQui = threading.Semaphore(3)
LockQui = threading.Lock()
EsperaQui = False
contQuiIn = 0
contQuiOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO DEF-----------------------------------

semFilaDef = threading.Semaphore(11)
semDepDef = threading.Semaphore(5)
LockDef = threading.Lock()
EsperaDef = False
contDefIn = 0
contDefOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO MEC-----------------------------------

semFilaMec = threading.Semaphore(13)
semDepMec = threading.Semaphore(4)
LockMec = threading.Lock()
EsperaMec = False
contMecIn = 0
contMecOut = 0

#LOCKS, SEMAFOROS, CONTADOR Y ESTADO DEPARTAMENTO MIN-----------------------------------

semFilaMin = threading.Semaphore(9)
semDepMin = threading.Semaphore(2)
LockMin = threading.Lock()
EsperaMin = False
contMinIn = 0
contMinOut = 0

#LOCKS DE ESCRITURA---------------------------------------------------------------------

LockEscEnt = threading.Lock()
LockEscMat = threading.Lock()
LockEscInf = threading.Lock()
LockEscFis = threading.Lock()
LockEscQui = threading.Lock()
LockEscDef = threading.Lock()
LockEscMec = threading.Lock()
LockEscMin = threading.Lock()
LockEscSal = threading.Lock()

#FUNCIONES DE ESCRITURA ENTRADA---------------------------------------------------------

def RegistrarEntrada(identificador, TiempoEntrada, depto1, TiempoDepto1, depto2, TiempoDepto2):
    with LockEscEnt:
        with open('PdLamparas.txt', 'a') as f:
            f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoEntrada).strftime("%H:%M:%S.%f")}, {depto1}, {datetime.datetime.fromtimestamp(TiempoDepto1).strftime("%H:%M:%S.%f")}, {depto2}, {datetime.datetime.fromtimestamp(TiempoDepto2).strftime("%H:%M:%S.%f")}\n')

#FUNCIONES DE ESCRITURA SALIDA----------------------------------------------------------

def RegistrarSalida(identificador, TiempoSalida):
    with LockEscSal:
        with open('Salida.txt', 'a') as f:
            f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoSalida).strftime("%H:%M:%S.%f")}\n')

#FUNCIONES DE ESCRITURA PARA DEPARTAMENTOS----------------------------------------------

def RegistrarDepto(identificador, TiempoFila, TiempoDepto, Num, depto):
    if(depto == "Matemáticas"):
        with LockEscMat:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')

    elif(depto == "Informática"):
        with LockEscInf:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')


    elif(depto == "Física"):
        with LockEscFis:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')
    
    elif(depto == "Química"):
        with LockEscQui:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')
    
    elif(depto == "DEFIDER"):
        with LockEscDef:
            with open(f'{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')
    
    elif(depto == "Mecánica"):
        with LockEscMec:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')
    
    elif(depto == "Minas"):
        with LockEscMin:
            with open(f'Departamento_de_{depto}.txt', 'a') as f:
                f.write(f'Persona{identificador}, {datetime.datetime.fromtimestamp(TiempoFila).strftime("%H:%M:%S.%f")}, {datetime.datetime.fromtimestamp(TiempoDepto).strftime("%H:%M:%S.%f")}, {Num}\n')

    else:
        print("Error de Escritura")

#FUNCION DE ENTRADA AL PATIO DE LAS LAMPARAS--------------------------------------------

def patiolamparas(identificador):
    TiempoEntrada = time.time()

    Departamento1 = random.randint(1, 7)
    Departamento2 = random.randint(1, 7)

    while(Departamento2==Departamento1):
        Departamento2 = random.randint(1, 7)

    if(Departamento1==1):
        depto1 = "Departamento de Matemáticas"
        TiempoEntradaDepto1 = deptomatematica(identificador,1)

    elif(Departamento1==2):
        depto1 = "Departamento de Informática"
        TiempoEntradaDepto1 = deptoinformatica(identificador,1)

    elif(Departamento1==3):
        depto1 = "Departamento de Física"
        TiempoEntradaDepto1 = deptofisica(identificador,1)

    elif(Departamento1==4):
        depto1 = "Departamento de Química"
        TiempoEntradaDepto1 = deptoquimica(identificador,1)

    elif(Departamento1==5):
        depto1 = "DEFIDER"
        TiempoEntradaDepto1 = defider(identificador,1)

    elif(Departamento1==6):
        depto1 = "Departamento de Mecánica"
        TiempoEntradaDepto1 = deptomecanica(identificador,1)

    elif(Departamento1==7):
        depto1 = "Departamento de Minas"
        TiempoEntradaDepto1 = deptominas(identificador,1)

    else:
        print("Error de seleccion de departamento 1")


    if(Departamento2==1):
        depto2 = "Departamento de Matemáticas"
        TiempoEntradaDepto2 = deptomatematica(identificador,2)

    elif(Departamento2==2):
        depto2 = "Departamento de Informática"
        TiempoEntradaDepto2 = deptoinformatica(identificador,2)

    elif(Departamento2==3):
        depto2 = "Departamento de Física"
        TiempoEntradaDepto2 = deptofisica(identificador,2)

    elif(Departamento2==4):
        depto2 = "Departamento de Química"
        TiempoEntradaDepto2 = deptoquimica(identificador,2)

    elif(Departamento2==5):
        depto2 = "DEFIDER"
        TiempoEntradaDepto2 = defider(identificador,2)

    elif(Departamento2==6):
        depto2 = "Departamento de Mecánica"
        TiempoEntradaDepto2 = deptomecanica(identificador,2)

    elif(Departamento2==7):
        depto2 = "Departamento de Minas"
        TiempoEntradaDepto2 = deptominas(identificador,2)

    else:
        print("Error de seleccion de departamento 2")

    TiempoSalida = time.time()
    RegistrarEntrada(identificador,TiempoEntrada,depto1,TiempoEntradaDepto1,depto2,TiempoEntradaDepto2)
    RegistrarSalida(identificador,TiempoSalida)

#FUNCION DE ENTRADA AL DEPARTAMENTO DE MATEMATICAS--------------------------------------

def deptomatematica(identificador, Num):
    global EsperaMat
    global contMatIn
    global contMatOut

    #FILA DEL DEPARTAMENTO DE MATEMATICA
    
    with semFilaMat:
        TiempoFila = time.time()
        with LockMat:
            contMatIn += 1
        
        while((contMatIn < 10) or (EsperaMat)):
            pass

    #ATENCION DEL DEPARTAMENTO DE MATEMATICA
        with semDepMat:
            with LockMat:
                contMatOut += 1
                if(contMatOut==10):
                    EsperaMat = True
                contMatIn -= 1

            TiempoDepto = time.time()
            time.sleep(9)

            with LockMat:
                contMatOut -= 1
                if(contMatOut == 0):
                    EsperaMat = False
    
    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Matemáticas")
    return TiempoFila

#FUNCION DE ENTRADA AL DEPARTAMENTO DE INFORMATICA--------------------------------------

def deptoinformatica(identificador, Num):
    global EsperaInf
    global contInfIn
    global contInfOut

    #FILA DEL DEPARTAMENTO DE INFORMATICA
    with semFilaInf:
        TiempoFila = time.time()
        with LockInf:
            contInfIn += 1
        
        while((contInfIn < 2) or (EsperaInf)):
            pass

    #ATENCION DEL DEPARTAMENTO DE INFORMATICA
        with semDepInf:
            with LockInf:
                contInfOut += 1
                if(contInfOut==2):
                    EsperaInf = True
                contInfIn -= 1

            TiempoDepto = time.time()
            time.sleep(5)

            with LockInf:
                contInfOut -= 1
                if(contInfOut == 0):
                    EsperaInf = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Informática")
    return TiempoFila

#FUNCION DE ENTRADA AL DEPARTAMENTO DE FISICA-------------------------------------------

def deptofisica(identificador, Num):
    global EsperaFis
    global contFisIn
    global contFisOut

    #FILA DEL DEPARTAMENTO DE FISICA
    
    with semFilaFis:
        TiempoFila = time.time()
        with LockFis:
            contFisIn += 1
        
        while((contFisIn < 5) or (EsperaFis)):
            pass

    #ATENCION DEL DEPARTAMENTO DE FISICA
        with semDepFis:
            with LockFis:
                contFisOut += 1
                if(contFisOut==5):
                    EsperaFis = True
                contFisIn -= 1

            TiempoDepto = time.time()
            time.sleep(7)

            with LockFis:
                contFisOut -= 1
                if(contFisOut == 0):
                    EsperaFis = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Física")
    return TiempoFila

#FUNCION DE ENTRADA AL DEPARTAMENTO DE QUIMICA------------------------------------------

def deptoquimica(identificador, Num):
    global EsperaQui
    global contQuiIn
    global contQuiOut

    #FILA DEL DEPARTAMENTO DE QUIMICA
    
    with semFilaQui:
        TiempoFila = time.time()
        with LockQui:
            contQuiIn += 1
        
        while((contQuiIn < 3) or (EsperaQui)):
            pass

    #ATENCION DEL DEPARTAMENTO DE QUIMICA
        with semDepQui:
            with LockQui:
                contQuiOut += 1
                if(contQuiOut==3):
                    EsperaQui = True
                contQuiIn -= 1

            TiempoDepto = time.time()
            time.sleep(4)

            with LockQui:
                contQuiOut -= 1
                if(contQuiOut == 0):
                    EsperaQui = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Química")
    return TiempoFila

#FUNCION DE ENTRADA AL DEFIDER----------------------------------------------------------

def defider(identificador, Num):
    global EsperaDef
    global contDefIn
    global contDefOut

    #FILA DEL DEPARTAMENTO DE DEFIDER
    
    with semFilaDef:
        TiempoFila = time.time()
        with LockDef:
            contDefIn += 1
        
        while((contDefIn < 5) or (EsperaDef)):
            pass

    #ATENCION DEL DEPARTAMENTO DE DEFIDER
        with semDepDef:
            with LockDef:
                contDefOut += 1
                if(contDefOut==5):
                    EsperaDef = True
                contDefIn -= 1

            TiempoDepto = time.time()
            time.sleep(1)

            with LockDef:
                contDefOut -= 1
                if(contDefOut == 0):
                    EsperaDef = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "DEFIDER")
    return TiempoFila

#FUNCION DE ENTRADA AL DEPARTAMENTO DE MECANICA-----------------------------------------

def deptomecanica(identificador, Num):
    global EsperaMec
    global contMecIn
    global contMecOut

    #FILA DEL DEPARTAMENTO DE MECANICA
    
    with semFilaMec:
        TiempoFila = time.time()
        with LockMec:
            contMecIn += 1
        
        while((contMecIn < 4) or (EsperaMec)):
            pass

    #ATENCION DEL DEPARTAMENTO DE MECANICA
        with semDepMec:
            with LockMec:
                contMecOut += 1
                if(contMecOut==4):
                    EsperaMec = True
                contMecIn -= 1

            TiempoDepto = time.time()
            time.sleep(4)

            with LockMec:
                contMecOut -= 1
                if(contMecOut == 0):
                    EsperaMec = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Mecánica")
    return TiempoFila

#FUNCION DE ENTRADA AL DEPARTAMENTO DE MINAS--------------------------------------------

def deptominas(identificador, Num):
    global EsperaMin
    global contMinIn
    global contMinOut

    #FILA DEL DEPARTAMENTO DE MINAS
    
    with semFilaMin:
        TiempoFila = time.time()
        with LockMin:
            contMinIn += 1
        
        while((contMinIn < 2) or (EsperaMin)):
            pass

    #ATENCION DEL DEPARTAMENTO DE MINAS
        with semDepMin:
            with LockMin:
                contMinOut += 1
                if(contMinOut==2):
                    EsperaMin = True
                contMinIn -= 1

            TiempoDepto = time.time()
            time.sleep(5)

            with LockMin:
                contMinOut -= 1
                if(contMinOut == 0):
                    EsperaMin = False

    RegistrarDepto(identificador,TiempoFila,TiempoDepto, Num, "Minas")
    return TiempoFila

#PROGRAMA PRINCIPAL---------------------------------------------------------------------

asistentes = list()

for identificador in range(500):
    persona = threading.Thread(target=patiolamparas,args=[identificador])
    asistentes.append(persona)
    persona.start()

for persona in asistentes:
    persona.join()