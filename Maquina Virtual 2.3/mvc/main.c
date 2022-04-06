#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funciones.h"

int main(int argc, char* argv[]){
    int Memoria[DimMemoria];
    int Registros[DimReg];
    char linea[200];
    Codigo Codigos[CantInstrucciones];
    vecRotulo vecRot;
    char rotulo[20],mnemonico[20],op1[20],op2[20],comentario[80];
    int i,celdaRam=0x0, codMnemonico, error, indice=1, generaArchivo=1, cantLineas = 0; //Si generaArchivo=0 entonces no se genera el Archivo

    //INICIALIZACION

    inicializaVector(Memoria,DimMemoria);
    inicializaVector(Registros,DimMemoria);
    inicializaRotulos(argv[1],&vecRot,&cantLineas); //Inicializa vector de rotulos/linea
    InicializaCodigos(Codigos); //Inicializa los mnemonicos en el vector
    vecConst.cantC=0; //Inicializa la cantidad de constantes
    vecConst.espacioTotal=0; //Inicializa el espacio adicional en el CS
    Registros[1]=0; //CS
    Registros[2]=cantLineas*3 + vecConst.espacioTotal; //DS
    Registros[3]=Registros[2]+ 500; //ES
    Registros[5]=Registros[3] + 500; //SS
    Registros[6]=500; //SP
    Registros[7]=Registros[5]+Registros[6]; //BP
    Registros[0]=Registros[2]+Registros[3]+Registros[5]+Registros[6]; //PS

    FILE *codigofuente, *archivoimg; //Archivo con codigo fuente Assembler que vamos a traducir a codigo maquina
    int o = 1;
    if(argc < 3){
        printf("faltan parametros para la ejecucion..");
        printf("mvc.exe <File.asm> <File.img> [-o]");
        exit(1);
    }else{
        if(argc > 4){
            printf("hay mas parametros de los necesarios.. error de ejecucion");
            exit(1);
        }else{
            codigofuente = fopen(argv[1],"rb"); //.asm
            if(argc == 4 && strcmp(argv[3],"-o")==0 ){
                o = 0;
            }
        }
    }
    if( codigofuente == NULL ){ //CONTROLA QUE EXISTA EL ARCHIVO
        printf("Hubo un problema al abrir un archivo..");
        exit(1);
    }

    strcpy(op1,"");
    strcpy(op2,"");
    strcpy(comentario,"");
    strcpy(rotulo,"");
    fgets(linea,sizeof(linea),codigofuente);
    while(!feof(codigofuente)){ //ARRANCA LA TRADUCCION
        error = 0;
        normalizaLinea(linea);
        if(linea[0]=='\\'){
            calculaDimRegistros(Registros,linea);
            if(o == 1)
                printf("\n%s",linea);
        }
        else{
            separaArgumentos(linea,rotulo,mnemonico,op1,op2,comentario);
            codMnemonico = detectaFuncion(mnemonico,op1,Codigos);
            if(codMnemonico < 0){
                if( codMnemonico == -1 ){ //Comentario UNICO
                    if(o == 1)
                        if( comentario[0] == '/' )
                            printf("\n%s",comentario);
                }
                else{ //codMenomico == -2 por lo que estoy en una constante.
                    agregaConstante(Memoria,&vecConst,mnemonico,op2,cantLineas*3); //BASE EQU @43
                    if(o == 1)
                        printf("\n%s %s %s",mnemonico,op1,op2);
                }
            }
            else{
                if( codMnemonico == 0 ){ //ERROR SINTAXIS
                    error = 1;
                    generaArchivo = 0;
                }
                else{ //Instruccion cualquiera
                    Memoria[celdaRam]=codMnemonico*0x10000+devuelveTipo(op1,codMnemonico)*0x100+devuelveTipo(op2,codMnemonico);
                    if(0x20 <= codMnemonico && codMnemonico <= 0x29){
                        Jump(Memoria,celdaRam,vecRot,op1,op2,codMnemonico,&error);
                    }
                    else{
                        Memoria[celdaRam+1]=operando(op1,codMnemonico);
                        Memoria[celdaRam+2]=operando(op2,codMnemonico);
                    }
                }
                muestraPantalla(Memoria,celdaRam,indice,rotulo,mnemonico,Codigos,op1,op2,comentario,o,error);
                celdaRam += 3;
                indice++;
            }
        }
        strcpy(op1,"");
        strcpy(op2,"");
        strcpy(comentario,"");
        strcpy(rotulo,"");
        fgets(linea,sizeof(linea),codigofuente);

    }//TERMINA LA TRADUCCION

    fclose(codigofuente);

    if( generaArchivo == 1 ){ //AGREGA EL REGISTRO DS
        char direccionImg[100];
        strcpy(direccionImg,"../../../mvx/bin/Debug/");
        strcat(direccionImg,argv[2]);
        archivoimg = fopen(direccionImg,"wb");   //.img
        for(i=0;i<DimReg;i++){
            fwrite(&(Registros[i]),sizeof(int),1,archivoimg);
        }
        for(i=0;i<DimMemoria;i++){
            fwrite(&(Memoria[i]),sizeof(int),1,archivoimg);
        }
        fclose(archivoimg);
    }
    return 0;
}
