#define DimMemoria 2000
#define DimReg 16
#define CantInstrucciones 27

typedef struct{ //struct de Rotulo / Linea
    char rotulo[30];
    int linea;
} rotulo;

typedef struct{ //struct de vector de rotulos/linea
    int cantR; //cuantos rotulos hay guardados
    rotulo rotulos[20];
} vecRotulo;

typedef struct{ //struct de Constantes y valor
    char constante[30];
    int valor;
} constante;

typedef struct{
    int cantC;
    int espacioTotal;
    constante constantes[20];
} vecConstante;

typedef struct{
    char mnemonico[5];
    int cod;
} Codigo;

vecConstante vecConst;

//CABECERAS

void normalizaLinea(char[]);

int detectaRotulo(char[]);

int detectaFuncion(char f[], char op1[], Codigo cod[]);

void separaArgumentos(char[], char[] ,char[], char[], char[], char[]);

void InicializaCodigos(Codigo[]);

int operando(char operando[], int cod);

int devuelveTipo(char op[], int cod);

void inicializaRotulos(char nombre[],vecRotulo *vecR, int *cantLineas);

void muestraPantalla(int Memoria[], int celdaRam, int indice,char rotulo[], char mnemonico[],Codigo codigos[], char ope1[], char ope2[], char comentario[], int o, int error);

void Jump(int Memoria[], int celdaRam, vecRotulo vecRot, char op1[], char op2[], int codMnemonico, int *error);

void inicializaVector(int vec[], int n);

void agregaConstante(int Memoria[], vecConstante *vecConst,char mnemonico[],char op2[],int CS);

void calculaDimRegistros(int Registros[], char linea[]);
