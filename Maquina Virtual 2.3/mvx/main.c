#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funciones.h"

int main(int argc, char* argv[]){
    int codigo,tipo1,tipo2,auxIP;
    void (*puntero_funcion[0x90])(int t1, int op1, int t2, int op2);
    Registros[IP] = 0;

    puntero_funcion[0x1] = MOV;
    puntero_funcion[0x2] = ADD;
    puntero_funcion[0x3] = SUB;
    puntero_funcion[0x4] = MUL;
    puntero_funcion[0x5] = DIV;
    puntero_funcion[0x6] = MOD;
    puntero_funcion[0x13] = CMP;
    puntero_funcion[0x17] = SWAP;
    puntero_funcion[0x19] = RND;
    puntero_funcion[0x31] = AND;
    puntero_funcion[0x32] = OR;
    puntero_funcion[0x33] = NOT;
    puntero_funcion[0x34] = XOR;
    puntero_funcion[0x37] = SHL;
    puntero_funcion[0x38] = SHR;
    puntero_funcion[0x20] = JMP;
    puntero_funcion[0x21] = JE;
    puntero_funcion[0x22] = JG;
    puntero_funcion[0x23] = JL;
    puntero_funcion[0x24] = JZ;
    puntero_funcion[0x25] = JP;
    puntero_funcion[0x26] = JN;
    puntero_funcion[0x27] = JNZ;
    puntero_funcion[0x28] = JNP;
    puntero_funcion[0x29] = JNN;
    puntero_funcion[0x81] = SYS;
    puntero_funcion[0x8F] = STOP;

    iniciaMaquina(argv[1],Registros,Memoria);

    if(argc == 3){
            if( strcmp(argv[2],"-d") == 0 ){
                flag();
            }
    }

    while( 0 <= Registros[IP] && Registros[IP] < Registros[DS] ){
      //  Debugger();
        auxIP = Registros[IP];
        divideOperandos(Memoria[Registros[IP]],&codigo,&tipo1,&tipo2);
        puntero_funcion[codigo](tipo1,Memoria[Registros[IP]+1],tipo2,Memoria[Registros[IP]+2]);
        if( auxIP == Registros[IP] ) //solo aumenta en 3 si no estoy realizando un salto
            Registros[IP] += 0x3;
    }

    return 0;
}
