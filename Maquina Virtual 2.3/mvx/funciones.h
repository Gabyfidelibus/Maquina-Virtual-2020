#define DimRegistros 16
#define DimMemoria 2000

#define DS 2
#define ES 3
#define IP 4
#define CC 9

int Registros[DimRegistros];

int Memoria[DimMemoria];

void generaVectorPunteros();

void iniciaMaquina(char nombre[],int Registros[], int Memoria[]);

void divideOperandos(int celda, int *codigo, int *tipo1, int *tipo2);

int devuelveValor(int tipo, int operando);

void setCC(int resultado);

void MOV(int t1, int op1, int t2, int op2);

void ADD(int t1, int op1, int t2, int op2);

void SUB(int t1, int op1, int t2, int op2);

void MUL(int t1, int op1, int t2, int op2);

void DIV(int t1, int op1, int t2, int op2);

void MOD(int t1, int op1, int t2, int op2);

void CMP(int t1, int op1, int t2, int op2);

void SWAP(int t1, int op1, int t2, int op2);

void RND(int t1, int op1, int t2, int op2);

void AND(int t1, int op1, int t2, int op2);

void OR(int t1, int op1, int t2, int op2);

void NOT(int t1, int op1, int t2, int op2);

void XOR(int t1, int op1, int t2, int op2);

void SHL(int t1, int op1, int t2, int op2);

void SHR(int t1, int op1, int t2, int op2);

void JMP(int t1, int op1, int t2, int op2);

void JE(int t1, int op1, int t2, int op2);

void JG(int t1, int op1, int t2, int op2);

void JL(int t1, int op1, int t2, int op2);

void JZ(int t1, int op1, int t2, int op2);

void JP(int t1, int op1, int t2, int op2);

void JN(int t1, int op1, int t2, int op2);

void JNZ(int t1, int op1, int t2, int op2);

void JNP(int t1, int op1, int t2, int op2);

void JNN(int t1, int op1, int t2, int op2);

void SYS(int t1, int op1, int t2, int op2);

void STOP(int t1, int op1, int t2, int op2);

void leer();

void escribir();

void dump();

void flag();

void Debugger();
