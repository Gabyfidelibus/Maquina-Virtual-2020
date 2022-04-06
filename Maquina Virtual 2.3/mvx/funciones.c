#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funciones.h"

void iniciaMaquina(char nombre[],int Registros[], int Memoria[]){
    FILE *arch;
    int i;

    arch=fopen(nombre,"rb");
    if(arch==NULL){
        printf("No se encontro el archivo.");
        exit(1);
    }
    else{
        for(i=0;i<DimRegistros;i++){
            fread(&(Registros[i]),sizeof(int),1,arch);
        }
         for(i=0;i<DimMemoria;i++){
             fread(&(Memoria[i]),sizeof(int),1,arch);
        }
    }

}

void divideOperandos(int celda, int *codigo, int *tipo1, int *tipo2){ //separa codigo de tipos (primera linea de instruccion)
    *codigo=celda/0x10000;
    *tipo1=(celda % 0x10000)/0x100;
    *tipo2=celda % 0x100;
}

int devuelveValor(int tipo, int operando){
    int aux;

    switch (tipo){
    case 0: aux=operando;
        break;
    case 1: aux=Registros[operando];
        break;
    case 2: if (operando<0x30000000)
                aux=Memoria[operando-0x20000000+Registros[DS]];
            else
                aux=Memoria[operando-0x30000000+Registros[ES]];
        break;
    }

    return aux;
}

void setCC(int resultado){
    if( resultado == 0 )
        Registros[CC] = 0x00000001;
    else if( resultado < 0 )
        Registros[CC] = 0x10000000;
    else
        Registros[CC] = 0x00000000;
}

void MOV(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]=devuelveValor(t2,op2);
        }
    }
}

void ADD(int t1, int op1, int t2, int op2){ //por ahora en este solo se implemento setCC
    if (t1 == 01){ //REGISTRO
        Registros[op1]+=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]+=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]+=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void SUB(int t1, int op1, int t2, int op2){
     if (t1 == 01){ //REGISTRO
        Registros[op1]-=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]-=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]-=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void MUL(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]*=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]*=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]*=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void DIV(int t1, int op1, int t2, int op2){
     if (t1 == 01){ //REGISTRO
        Registros[op1]/=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]/=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]/=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void MOD(int t1, int op1, int t2, int op2){
     if (t1 == 01){ //REGISTRO
        Registros[op1]%=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]%=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]%=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void CMP(int t1, int op1, int t2, int op2){
    setCC(devuelveValor(t1,op1)-devuelveValor(t2,op2));
}

void SWAP(int t1, int op1, int t2, int op2){
    int aux;

    if(t1 == 01 && t2 == 01){
        aux = Registros[op1];
        Registros[op1] = Registros[op2];
        Registros[op2] = aux;
    }
    else if(op1<0x30000000){
        aux = Memoria[op1-0x20000000+Registros[DS]];
        if(op2<0x30000000)
            Memoria[op1-0x20000000+Registros[DS]]=Memoria[op2-0x20000000+Registros[DS]];
        else
            Memoria[op1-0x20000000+Registros[DS]]=Memoria[op1-0x30000000+Registros[ES]];
        Memoria[op2-0x20000000+Registros[DS]]=aux;
    }
    else if(op1>=0x30000000){
        aux = Memoria[op1-0x30000000+Registros[ES]];
        if(op2<0x30000000)
            Memoria[op1-0x30000000+Registros[ES]]=Memoria[op2-0x20000000+Registros[DS]];
        else
            Memoria[op1-0x30000000+Registros[ES]]=Memoria[op1-0x30000000+Registros[ES]];
        Memoria[op2-0x30000000+Registros[ES]]=aux;
    }
}

void RND(int t1, int op1, int t2, int op2){
    if (t1 == 01){
        Registros[op1]=rand()%(devuelveValor(t2,op2)+1);
    }
    else{
        if (op1<0x30000000)
            Memoria[op1-0x20000000+Registros[DS]]=rand()%(devuelveValor(t2,op2)+1);
        else{
            Memoria[op1-0x30000000+Registros[ES]]=rand()%(devuelveValor(t2,op2)+1);
        }
    }
}

void AND(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]&=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]&=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]&=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void OR(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]|=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]|=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]|=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void NOT(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]=-Registros[op1];
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000){ //DS
            Memoria[op1-0x20000000+Registros[DS]]=-Memoria[op1-0x20000000+Registros[DS]];
        }else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]=-Memoria[op1-0x30000000+Registros[ES]];
        }
    }
    setCC(devuelveValor(t1,op1));
}

void XOR(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]^=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]^=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]^=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void SHL(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]<<=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]<<=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]<<=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void SHR(int t1, int op1, int t2, int op2){
    if (t1 == 01){ //REGISTRO
        Registros[op1]>>=devuelveValor(t2,op2);
    }
    else{ //CELDA DE MEMORIA
        if (op1<0x30000000) //DS
            Memoria[op1-0x20000000+Registros[DS]]>>=devuelveValor(t2,op2);
        else{ //ES
            Memoria[op1-0x30000000+Registros[ES]]>>=devuelveValor(t2,op2);
        }
    }
    setCC(devuelveValor(t1,op1));
}

void JMP(int t1, int op1, int t2, int op2){
    Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JE(int t1, int op1, int t2, int op2){
    if (Registros[0xA] == devuelveValor(t1,op1))
        Registros[IP]=devuelveValor(t2,op2)*3 - 3;
}

void JG(int t1, int op1, int t2, int op2){
    if (Registros[0xA] < devuelveValor(t1,op1))
        Registros[IP]=devuelveValor(t2,op2)*3 - 3;
}

void JL(int t1, int op1, int t2, int op2){
    if (Registros[0xA] > devuelveValor(t1,op1))
        Registros[IP]=devuelveValor(t2,op2)*3 - 3;
}

void JZ(int t1, int op1, int t2, int op2){
    if (Registros[CC] == 0x00000001)
        Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JP(int t1, int op1, int t2, int op2){
    if (Registros[CC] == 0x00000000)
        Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JN(int t1, int op1, int t2, int op2){
    if (Registros[CC] == 0x10000000)
        Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JNZ(int t1, int op1, int t2, int op2){
    if( (Registros[CC] == 0x10000000) || (Registros[CC] == 0x00000000) )
        Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JNP(int t1, int op1, int t2, int op2){
    if (Registros[CC] == 0x10000000 || Registros[CC] == 0x00000001)
        Registros[IP]=devuelveValor(t1,op1)*3 - 3;
}

void JNN(int t1, int op1, int t2, int op2){
    if (Registros[CC] == 0x00000000 || Registros[CC] == 0x00000001)
        Registros[IP]= devuelveValor(t1,op1)*3 - 3;
}

void STOP(int t1, int op1, int t2, int op2){
    Registros[IP] = Registros[DS];
}

void SYS(int t1, int op1, int t2, int op2){

    switch(op1){
    case 1:
        leer();
        break;
    case 2:
        escribir();
        break;
    case 3:
        dump();
        break;
    }
}

void leer(){
    int i,posicionDato;
    char s[100];

    posicionDato = Registros[0xd];
    if( Registros[0xd] <= 1000 )
        posicionDato += Registros[DS];

    if( (Registros[0xA] & 0b100000000) == 0b100000000){ //Lectura caracter x caracter
        //caracter a caracterr
        if( (Registros[0xA] & 0b1000000000000) == 0b0000000000000 ){
            printf("[%04x] ",Registros[0xD]);
        }
        scanf("%s",s);
        i = 0;
        while( i < Registros[0xc] && s[i] != '\0' ){
            if( s[i] <= 48 || s[i] <= 57 )
                Memoria[posicionDato+i] = (int)(s[i]-48);
            else
                Memoria[posicionDato+i] = s[i];
            i++;
        }
    }
    else{ //lectura de valores por "enter" (tantos como diga Reg[0xc]
        for( i=0; i<Registros[0xc]; i++ ){
            if( (Registros[0xA] & 0b1000000000000) == 0b0000000000000){
                if( Registros[0xd] <= 1000 )
                printf("[%04x] ",posicionDato+i-Registros[DS]);
            else
                printf("[%04x] ",posicionDato+i);
               // printf("[%04d]: ",posicionDato+i);
            }
            if( ( Registros[0xa] & 0b1000) == 0b1000 ){ //hexa 1000
                scanf("%x",&Memoria[posicionDato+i]);
            }else
            if( (Registros[0xa] & 0b0100) == 0b0100 ){ //octal 0100
                scanf("%o",&Memoria[posicionDato+i]);
            }else
            if( (Registros[0xa] & 0b0001) == 0b0001 ){ //decimal 0001
                scanf("%d",&Memoria[posicionDato+i]);
            }else{
                scanf("%d",&Memoria[posicionDato+i]); // lee decimal por defecto
            }
        }
    }
}

void escribir(){
    int i,posicionDato;

    posicionDato = Registros[0xd];
    if( 1000 >= Registros[0xd] )
        posicionDato += Registros[DS];

    for(i=0;i<Registros[0xC];i++){
        if((Registros[0xA] & 0b1000000000000) == 0b0000000000000){
                printf("[%04d]: ",Registros[0xd]+i);
        }
            if( ( Registros[0xa] & 0b10000) == 0b10000 ){ //caracter 10000
                if((Memoria[posicionDato+i])>=0x20)
                    printf("%c ",Memoria[posicionDato+i]);
                else
                    printf(". ");
            }

            if( ( Registros[0xa] & 0b1000) == 0b1000 ){ //hexa 1000
                printf("&%x ",Memoria[posicionDato+i]);
            }

            if( ( Registros[0xa] & 0b0100 ) == 0b0100 ){ //octal 0100
                printf("@%o ",Memoria[posicionDato+i]);
            }

            if( (Registros[0xa] & 0b0001) == 0b0001 ){ //decimal 0001
                printf("%d ",Memoria[posicionDato+i]);
            }
        if( (Registros[0xA] & 0b100000000 ) == 0b000000000){
            printf("\n");
        }
    }
}

void dump(){
    char* nombresReg[17] = {"","","DS","ES","IP","","","","AC","CC","AX","BX","CX","DX","EX","FX"};

    printf("\n");
    for(int i=0; i < DimRegistros; i++){

            if( strcmp(nombresReg[i],"") != 0 ){
                printf("[%s]: ",nombresReg[i]);

                if( ( Registros[0xa] & 0b1000) == 0b1000 ){ //hexa 1000
                    printf("&%x ",Registros[i]);
                }

                if( ( Registros[0xa] & 0b0100 ) == 0b0100 ){ //octal 0100
                    printf("@%o ",Registros[i]);
                }

                if( (Registros[0xa] & 0b0001) == 0b0001 ){ //decimal 0001
                    printf("%d ",Registros[i]);
                }

                printf("\n");
            }
    }

    printf("\n--------------------------------------------------------\n");

    escribir();
}

void flag(){ //se ejecuta al indicar la bandera [-d] por terminal

    printf("\n**********************FLAG************************\n\n");

    char* mnemonico[0x90];

    mnemonico[0x1] = "MOV";
    mnemonico[0x2] = "ADD";
    mnemonico[0x3] = "SUB";
    mnemonico[0x4] = "MUL";
    mnemonico[0x5] = "DIV";
    mnemonico[0x6] = "MOD";
    mnemonico[0x13] = "CMP";
    mnemonico[0x17] = "SWAP";
    mnemonico[0x19] = "RND";
    mnemonico[0x31] = "AND";
    mnemonico[0x32] = "OR";
    mnemonico[0x33] = "NOT";
    mnemonico[0x34] = "XOR";
    mnemonico[0x37] = "SHL";
    mnemonico[0x38] = "SHR";
    mnemonico[0x20] = "JMP";
    mnemonico[0x21] = "JE";
    mnemonico[0x22] = "JG";
    mnemonico[0x23] = "JL";
    mnemonico[0x24] = "JZ";
    mnemonico[0x25] = "JP";
    mnemonico[0x26] = "JN";
    mnemonico[0x27] = "JNZ";
    mnemonico[0x28] = "JNP";
    mnemonico[0x29] = "JNN";
    mnemonico[0x81] = "SYS";
    mnemonico[0x8F] = "STOP";

    char* nombresReg[16] = {"","","DS","ES","IP","","","","AC","CC","AX","BX","CX","DX","EX","FX"};
    int codOpe,operando[3],tipo[3],max, celdaRam = 0x0;

    for(int i=0; i<Registros[DS]; i+=3){
        //MOSTRAR CELDA RAM
        printf("[%04x]: ",celdaRam);

        //MOSTRAR CODIGO MAQUINA
        printf("%08x %08x %08x %d: ",Memoria[i],Memoria[i+1],Memoria[i+2],celdaRam/3);

        codOpe = Memoria[i] / 0x10000;
        tipo[1] = (Memoria[i] % 0x1000) / 0x100;
        tipo[2] = (Memoria[i] % 0x10);
        operando[1] = Memoria[i+1];
        operando[2] = Memoria[i+2];

        printf("%s ",mnemonico[codOpe]);

        if( codOpe != 0x8F ){
            if( codOpe != 0x33 && codOpe != 0x20 && (codOpe < 0x24 || codOpe > 0x29) && codOpe != 0x81){ //entra si tiene 2 operandos
                max = 3;
            }else{ //tiene un solo operando
                max = 2;
            }
        }else{
            max = 1;
        }
        for(int j=1; j<max; j++){
            switch(tipo[j]){
                case 0:
                    printf("%d",operando[j]);
                    break;
                case 1:
                    printf("%s",nombresReg[operando[j]]);
                    break;
                case 2:
                    printf("[");
                    if( operando[j] >= 0x30000000 ){
                        printf("ES:");
                        operando[j] -= 0x30000000;
                    }else{
                        operando[j] -= 0x20000000;
                    }
                    printf("%d]",operando[j]);
                    break;
            }
            if( max == 3 && j == 1)
                printf(", ");
        }
        printf("\n");
        celdaRam += 0x3;
    }
    printf("\n**************************************************\n\n");
}

//FUNCION PARA DEBUGGEAR CUANDO HAY ALGUN PROBLEMA

void Debugger(){
    printf("**DEBUGGER**\n");
    printf("IP: %08x\n",Registros[IP]);
    printf("CC: %08x\n",Registros[CC]);
    printf("AX: %08x\n",Registros[0xA]);
    printf("BX: %08x\n",Registros[0xB]);
    printf("CX: %08x\n",Registros[0xC]);
    printf("DX: %08x\n",Registros[0xD]);
    printf("EX: %08x\n",Registros[0xE]);
 //   printf("EF: %08x\n",Registros[0xF]);*/
 //   printf("[0]: %08x\n",Memoria[Registros[DS]]);
    printf("[1]: %08x\n",Memoria[Registros[DS]+1]);
 //   printf("[2]: %08x\n",Memoria[Registros[DS]+2]);
 //   printf("[3]: %08x\n",Memoria[Registros[DS]+3]);
    printf("-----------------------------\n");
}
