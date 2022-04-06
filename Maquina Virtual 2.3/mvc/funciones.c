#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "funciones.h"

void guardaCTE(int Memoria[], int CS, char op2[]){
    int i;
    //printf("\nPosicion: %d",CS+vecConst.espacioTotal);
    for(i=1; i<strlen(op2)-1; i++){
        Memoria[CS+vecConst.espacioTotal+i-1]=op2[i];
        //printf("\nMEMORIA:%d=%d %c",CS+vecConst.espacioTotal+i-1,Memoria[CS+vecConst.espacioTotal+i-1],op2[i]);
    }
    Memoria[i]='\0';
}

void agregaConstante(int Memoria[], vecConstante *vecConst,char mnemonico[],char op2[],int CS){
    (*vecConst).cantC++;
    strcpy((*vecConst).constantes[(*vecConst).cantC].constante,mnemonico);
    if(op2[0]!='"'){
        (*vecConst).constantes[(*vecConst).cantC].valor=operando(op2,devuelveTipo(op2,-2));
    }
    else{
        guardaCTE(Memoria,CS,op2);
        (*vecConst).constantes[(*vecConst).cantC].valor=CS+(*vecConst).espacioTotal;
        (*vecConst).espacioTotal+=strlen(op2)-1;

    }
}

void inicializaRotulos(char nombre[],vecRotulo *vecR, int *cantLineas){
    FILE *arch;
    char linea[100];
    int i,posR,celdaRam=1;
    arch = fopen(nombre,"rb");
    (*vecR).cantR = 0;
    fgets(linea,sizeof(linea),arch);
    while(!feof(arch)){
        normalizaLinea(linea);
        if((linea[0]!='/') && (strcmp(linea,"") != 0) && (strstr(strupr(linea)," EQU ")==NULL))
            *cantLineas+=1;
        posR=detectaRotulo(linea);
        if(posR!=0){
            (*vecR).rotulos[(*vecR).cantR].linea=celdaRam;
            for(i=0;i<=posR-3;i++){
                (*vecR).rotulos[(*vecR).cantR].rotulo[i]=linea[i];
            }
            (*vecR).rotulos[(*vecR).cantR].rotulo[i]='\0';
            (*vecR).cantR++;
        }
        if(linea[0] != '/' && strcmp(linea,"")!=0)
            celdaRam+=1;
        fgets(linea,sizeof(linea),arch);
    }
    fclose(arch);
}

void InicializaCodigos(Codigo Codigos[]){
    strcpy(Codigos[0].mnemonico,"MOV");
    Codigos[0].cod=0x01;
    strcpy(Codigos[1].mnemonico,"ADD");
    Codigos[1].cod=0x02;
    strcpy(Codigos[2].mnemonico,"SUB");
    Codigos[2].cod=0x03;
    strcpy(Codigos[3].mnemonico,"MUL");
    Codigos[3].cod=0x04;
    strcpy(Codigos[4].mnemonico,"DIV");
    Codigos[4].cod=0x05;
    strcpy(Codigos[5].mnemonico,"MOD");
    Codigos[5].cod=0x06;
    strcpy(Codigos[6].mnemonico,"CMP");
    Codigos[6].cod=0x13;
    strcpy(Codigos[7].mnemonico,"SWAP");
    Codigos[7].cod=0x17;
    strcpy(Codigos[8].mnemonico,"RND");
    Codigos[8].cod=0x19;
    strcpy(Codigos[9].mnemonico,"JMP");
    Codigos[9].cod=0x20;
    strcpy(Codigos[10].mnemonico,"JE");
    Codigos[10].cod=0x21;
    strcpy(Codigos[11].mnemonico,"JG");
    Codigos[11].cod=0x22;
    strcpy(Codigos[12].mnemonico,"JL");
    Codigos[12].cod=0x23;
    strcpy(Codigos[13].mnemonico,"JZ");
    Codigos[13].cod=0x24;
    strcpy(Codigos[14].mnemonico,"JP");
    Codigos[14].cod=0x25;
    strcpy(Codigos[15].mnemonico,"JN");
    Codigos[15].cod=0x26;
    strcpy(Codigos[16].mnemonico,"JNZ");
    Codigos[16].cod=0x27;
    strcpy(Codigos[17].mnemonico,"JNP");
    Codigos[17].cod=0x28;
    strcpy(Codigos[18].mnemonico,"JNN");
    Codigos[18].cod=0x29;
    strcpy(Codigos[19].mnemonico,"AND");
    Codigos[19].cod=0x31;
    strcpy(Codigos[20].mnemonico,"OR");
    Codigos[20].cod=0x32;
    strcpy(Codigos[21].mnemonico,"NOT");
    Codigos[21].cod=0x33;
    strcpy(Codigos[22].mnemonico,"XOR");
    Codigos[22].cod=0x34;
    strcpy(Codigos[23].mnemonico,"SHL");
    Codigos[23].cod=0x37;
    strcpy(Codigos[24].mnemonico,"SHR");
    Codigos[24].cod=0x38;
    strcpy(Codigos[25].mnemonico,"SYS");
    Codigos[25].cod=0x81;
    strcpy(Codigos[26].mnemonico,"STOP");
    Codigos[26].cod=0x8f;
}

int detectaRotulo(char linea[]){ //devuelve la posicion donde comienza el mnemonico, si no hay rotulo sera pos = 0
    int i,largo=strlen(linea);

    i=0;
    while(i<=largo && linea[i]!=' ' && linea[i]!=':' && linea[i]!='/')
        i++;

    if(linea[i]==':')
        return i+2;
    else
        return 0;
}

int detectaFuncion(char f[], char op1[], Codigo cod[]){
    int respuesta;
    int i=0;
    if(strcmp(strupr(op1),"EQU")==0)
        respuesta=-2;
    else{
        if( strcmp(f,"") != 0 ){
            while(i<=27 && strcmp(strupr(f),cod[i].mnemonico) != 0)
                i++;
            if(strcmp(strupr(f),cod[i].mnemonico)==0)
                respuesta = cod[i].cod;
            else
                respuesta = 0;
        }else{
            respuesta = -1;
        }
    }
    return respuesta;
}

void normalizaLinea(char linea[]){ //lee un string y lo normaliza (linea del programa assembler)
    int i,k,largo=strlen(linea);
    char aux[200];
    int comentario=0;
    k=0;
    while(linea[k]!=13 && linea[k]!='\n') //GABIGOD
        k++;
    linea[k]='\0';
    k=0;  //k es el indice que controla el string aux
    for(i=0;i<=largo;i++){
        if(comentario!=-1){
            if(i<largo && linea[i]!=' ' && linea[i]!='\t' && linea[i]!=',' && linea[i]!= '/' && linea[i]!= '='){
                if(i<largo && (linea[i+1] == ' ' || linea[i+1]==',' || linea[i+1]=='\t' || linea[i+1]=='=')){
                    aux[k++]=linea[i];
                    aux[k++]=' ';
                }
                else
                    aux[k++]=linea[i];
            }
            else{
                if(linea[i]=='/'){
                    comentario=-1;
                    aux[k++]='/';
                }
            }
        }
        else
            aux[k++]=linea[i];
    }
    strcpy(linea,aux);
    //printf("Linea normalizada: %s\n",linea);
}

void separaArgumentos(char linea[], char rotulo[] ,char mnemonico[], char op1[], char op2[], char comentario[]){
    int i=detectaRotulo(linea),largo=strlen(linea);
    int j=0,r=0,m=0,c=0,o1=0,o2=0;
    int control=0;

    if(i!=0){
        while(j<i-2){
            rotulo[r++]=linea[j++];
        }
    }

    j=i;

    while(j<=largo){
        if(linea[j]!='/'){
            if(linea[j]== ' ' && control!=-1)
                control+=1;
            else{
                switch(control){
                    case -1: comentario[c++]=linea[j];
                    break;
                    case 0: mnemonico[m++]=linea[j];
                    break;
                    case 1: op1[o1++]=linea[j];
                    break;
                    case 2: op2[o2++]=linea[j];
                    break;
                }
            }
        }
        else{//ENCONTRE COMENTARIO
            control=-1;
            comentario[c++]='/';
        }
        j++;
    }
    rotulo[r]='\0';
    mnemonico[m]='\0';
    op1[o1]='\0';
    op2[o2]='\0';
    comentario[c]='\0';
}

int buscaConstante(char op[]){ //Identifica si el operando es una CTE y si lo es devuelve su valor
    int tope=vecConst.cantC,i=0,rta=0;

    while(i<tope && strcmp(vecConst.constantes[i].constante,op)!=0)
        i++;

    if(strcmp(vecConst.constantes[i].constante,op)==0)
        rta=vecConst.constantes[i].valor;

    return rta;
}

int operando(char operando[], int cod){ //devuelve el hexa correspondiente (que se escribe en las lineas 2 y 3) de un operando
    int respuesta = buscaConstante(operando);   //Si es una CONSTANTE SALTEA TODO Y RETORNA SU VALOR YA GUARDADO
    if(respuesta!=0)
        return respuesta;

    switch(devuelveTipo(operando,cod)){ //tipoOperando(operando)
        case 0: //inmediato
            //% = hexa / @ = octal / # o vacio = decimal / ' o '' = ascii
            switch(operando[0]){
            case '@':
                sscanf(operando+1,"%o",&respuesta);
                break;
            case '#':
                sscanf(operando+1,"%d",&respuesta);
                break;
            case '%':
                sscanf(operando+1,"%x",&respuesta);
                break;
            case '\'':
                respuesta = operando[1];
                break;
            default:
                respuesta = atoi(operando);
                break;
            }
            break;
        case 1: //de registro
            if( toupper(operando[1]) == 'X' )
                sscanf(&(operando[0]),"%x",&respuesta);
            else
                if( toupper(operando[0]) == 'A' ) // es acumulador (ac)
                    respuesta = 0x8;
                else
                    respuesta = 0x9;
        break;
        case 2: //directo [celda] / [DS:celda] / [ES:celda]
            if(strstr(operando,"DS") != NULL){
                respuesta = atoi(operando+4) + 0x20000000; //saltea [DS: y va al numero
            }else if(strstr(operando,"ES") != NULL){
                respuesta = atoi(operando+4) + 0x30000000;
            }else{
                respuesta = atoi(operando+1) + 0x20000000; //el +1 hace que empiece despues del [
            }
        break;
        case 3: //indirecto MOV AX [DS:AX+1]

        break;
    }
    return respuesta;
}

int devuelveTipo(char op[], int cod){
    int r=0;
    if(strcmp(op,"")!=0 && cod!=0x81){
        if(op[0] == '[')
            if(strstr(strupr(op),"X")!=NULL || strstr(strupr(op),"SP")!=NULL || strstr(strupr(op),"BP")!=NULL)
                r=3;
            else
                r=2;
        else
            if(toupper(op[1])=='X' && strlen(op) == 2)
                r=1;
            else
                if((toupper(op[0])=='A' && toupper(op[1])=='C') || (toupper(op[0])=='C' && toupper(op[1])=='C') ) //consideracion para AC y CC
                    r=1;
    }
    return r;
}

void Jump(int Memoria[], int celdaRam, vecRotulo vecRot, char op1[], char op2[], int codMnemonico, int *error){
    int i = 0;

    if(codMnemonico <= 0x23 && codMnemonico >=0x21){ // JG NRO ROT
        while( i < vecRot.cantR-1 && strcmp(op2,vecRot.rotulos[i].rotulo) != 0){
            i++;
        }
        if(strcmp(op2,vecRot.rotulos[i].rotulo) == 0){
            Memoria[celdaRam+1]=operando(op1,codMnemonico);
            Memoria[celdaRam+2]=vecRot.rotulos[i].linea;
        }
        else{
            if( operando(op2,codMnemonico) > 0 ){
                Memoria[celdaRam+1]=operando(op1,codMnemonico);
                Memoria[celdaRam+2]=(operando(op2,codMnemonico));
            }else{
                *error=2;
            }
        }
    }
    else{ // JMP NRO ---
        while( i < vecRot.cantR-1 && strcmp(op1,vecRot.rotulos[i].rotulo) != 0)
            i++;
        if(strcmp(op1,vecRot.rotulos[i].rotulo) == 0){
            Memoria[celdaRam+1]=vecRot.rotulos[i].linea;
            Memoria[celdaRam+2]=0;
        }
        else{
            if( operando(op1,codMnemonico) > 0 ){
                Memoria[celdaRam+1]=(operando(op1,codMnemonico));
                Memoria[celdaRam+2]=operando(op2,codMnemonico);
            }else{
                *error=2;
            }
        }
    }
}

void muestraPantalla(int Memoria[], int celdaRam, int indice,char rotulo[], char mnemonico[],Codigo codigos[], char ope1[], char ope2[], char comentario[], int o, int error){
    if( o == 1 ){
        printf("\n[%08x]: ",celdaRam);
        if( error == 0 ){
            printf("%08x %08x %08x",Memoria[celdaRam],Memoria[celdaRam+1],Memoria[celdaRam+2]);
            if(strcmp(rotulo,"")!=0){
                printf("\t%s: ",rotulo);
            }
            else
                printf("\t%d: ",indice);
            if(strcmp(ope2,"")==0)
                printf("%s %s %s",mnemonico,ope1,comentario);
            else
                printf("%s %s,%s %s",mnemonico,ope1,ope2,comentario);
        }
        else
            if(error == 1)
                printf("FFFFFFFF FFFFFFFF FFFFFFFF");
            else
                printf("FFFFFFFF");
    }
}

void inicializaVector(int vec[], int n){
    int i;
    for(i=0;i<n;i++){
        vec[i]=0;
    }
}

void calculaDimRegistros(int Registros[], char linea[]){
    int i,DATA = 500, EXTRA = 500, STACK = 500;

    // Al normalizar se borran los espacios de mas
    i=6; // Donde empieza la asigancion de tamaños
    while(linea[i]!='\0'){
        if(toupper(linea[i])=='D'){ // Tamaño del DATA
            i+=5;
            DATA=0;
            while(linea[i]!=' ' && linea[i]!='\0'){
                DATA=10*DATA+linea[i++]-48;
            }
            if (linea[i]!='\0')
                i++;
        }
        else if(toupper(linea[i])=='E'){ // Tamaño del EXTRA
            i+=6;
            EXTRA=0;
            while(linea[i]!=' ' && linea[i]!='\0'){
                if(linea[i]=='-'){
                    EXTRA=-(linea[++i]-48);
                    i++;
                }
                else{
                    EXTRA=10*EXTRA+linea[i++]-48;
                }
            }
            if (linea[i]!='\0')
                i++;
        }
        else if(toupper(linea[i])=='S'){ // Tamaño del STACK
            i+=6;
            STACK=0;
            while(linea[i]!=' ' && linea[i]!='\0'){
                STACK=10*STACK+linea[i++]-48;
            }
            if (linea[i]!='\0')
                i++;
        }
        else
            i++;
    }

    if(EXTRA != -1){
        Registros[3]=Registros[2]+ DATA; //ES
        Registros[5]=Registros[3] + EXTRA; //SS
        Registros[0]=Registros[2]+ DATA + EXTRA + STACK; //PS
    }
    else{
        Registros[3]=EXTRA; //ES
        Registros[5]=Registros[2]+ DATA; //SS
        Registros[0]=Registros[2]+ DATA + STACK; //PS
    }
    Registros[6]=STACK; //SP
    Registros[7]=Registros[5]+Registros[6]; //BP
}
