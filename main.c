
#include   <stdio.h>
#include  <stdlib.h>
#include  <locale.h>
#include <stdbool.h>
#include  <string.h>
#include <windows.h>
#include   <ctype.h>

const int anoAtual = 2022;
const int tam      =   60;
const int linha    =   80;

typedef struct{
	char *txt;
	char formLinha;
	int  cor;
}TITULO;

const TITULO titulos[] = {
		{"< BEM VINDO AO CORONA.CAD >"                , '=',  46},// 0
	    {"< LOGIN >"                                  , '~',  27},// 1
	    {"< OPÇÕES >"                                 , '-',  60},// 2
	    {"< CADASTRO >"                               , '~', 105},// 3
	    {"< DADOS CADASTRADOS >"                      , '~', 143},// 4
	    {"< VOLTE SEMPRE >"                           , '=', 159},// 5
	    {"< ENDEREÇO >"                               , '~', 105},// 6
	    {"< EMAIL >"                                  , '~', 105},// 7
	    {"< DATA DE NASCIMENTO >"                     , '~', 105},// 8
	    {"< DATA DO DIAGNÓSTICO >"                    , '~', 105},// 9
	    {"< COMORBIDADE >"                            , '~', 105},//10
	    {"< VERIFICANDO LOGIN >"                      , '*', 185},//11
	    {"< CADASTRO SALVO COM SUCESSO >"             , '*', 160},//12
	    {"< | X | ERRO. DIGITE APENAS S OU N >"       , '-',  79},//13
	    {"< | X | ERRO. DIGITE UMA OPÇÃO VÁLIDA >"    , '-',  79},//14
	    {"< | X | ERRO. NENHUM ARQUIVO PARA LEITURA >", '-',  79},//15
	    {"< PACIENTES CADASTRADOS >"                  , '=',  46},//16
	    {"< GRUPO DE RISCO >"                         , '=',  46},//17
	};

typedef struct{
	int d;//dia
	int m;//mes
	int a;//ano
}DATA;

typedef struct{ 
	char           nome[50];
	char            rua[50];
	char         bairro[10];
	char         cidade[20];
	char         estado[20];
	char          email[50];
	char         comorb[20];
	bool     temComorbidade;
	double              cpf;
	double              tel;
	int                 num;
	double              cep;
	DATA               nasc;
	DATA               diag;
}CADASTRO;

void limparTela();
void congelar();
void setColor(const int cor);
void pularLinha();
void desenharLinha(char tam, char form, int cor);
void pausar();
void printfVoidLine();
void printCenter(char *msg);
void mostrarTitulo( TITULO t );
void mostrarErro( int numErro );
int lerInt(char *msg);
double lerDouble(char *msg);
char lerChar(char *msg);
int mostrarOpcs(char *mat[], int tam);
int getLogin();
int mostrarDados(CADASTRO c);
void desenharLinhaFile( FILE *a);
int salvarCad(CADASTRO c);
int salvarPacientesGrupRisc(int idade, int cep);
CADASTRO cadastrar();
int consultarArquivos( char *txt, int index_arq );
int consultar();
int sair();

int main(){
	
	CADASTRO                                          cadPacientes;
	int                                                      idade;
	int                                                        opc;
	int                                                       cont;
	char     *menuOpcs[] = {"CADASTRAR NOVO", "CONSULTAR", "SAIR"};
	bool     run         =                                    true;
	
	int index_boasVindas = 0;
	
	//ROTINA DE COMEÇO DO PROGRAMA ---------------------------->
	setlocale( LC_ALL, "portuguese");
	mostrarTitulo( titulos[ index_boasVindas ] );
	congelar( );
	limparTela( );
	getLogin( );
	//--------------------------------------------------------->
	
		
	//LOOP PARA VÁRIOS CADASTROS
	while (run){
		
		opc = mostrarOpcs( menuOpcs, 3 );
		
		switch (opc){
			case 1:
				cadPacientes = cadastrar();
				mostrarDados( cadPacientes );
				salvarCad( cadPacientes );
				idade = ( anoAtual - (cadPacientes.nasc.a));
	
				if (idade > 65 && cadPacientes.temComorbidade){
					salvarPacientesGrupRisc(idade, cadPacientes.cep);
				}
				
				break;
			case 2:
				consultar();
				break;
			case 3:
				sair();
				run = false;
				break;
		}
	}
	
	return 0;
}

void limparTela(){
	system("cls");
}

void congelar(){
	sleep(4);
}

void setColor(const int cor){
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), cor);
}

void pularLinha(){
	printf("\n");
}

void desenharLinha(char tam, char form, int cor){
	int c;
	setColor(cor);
	printf("+");
	for (c=1; c<=tam; c++){
		printf("%c", form);
	}
	printf("+\n");
}

void pausar(){
	pularLinha();
	system("pause");
	pularLinha();
}

void printfVoidLine(){
	int c;
	printf("|");
	for (c = 1; c <= linha; c++){
		printf(" ");
	}
	printf("|\n");
}

void printCenter(char *msg){
	int c;
	int tab = 20;
	int tamanho = strlen(msg);
	int esp1 = ( tamanho + tab);
	int esp2 = ( linha - esp1 );
	
	printf("|");
	for (c=1; c<=tab; c++){
		printf(" ");
	}
	
	printf("%s", msg);
	
	for (c=1; c<=esp2; c++){
		printf(" ");
	}
	printf("|\n");
}

void mostrarTitulo( TITULO t ){
	int tamanho = linha;
	desenharLinha( tamanho, t.formLinha, t.cor);
	printfVoidLine();
	printCenter( t.txt );
	printfVoidLine();
	desenharLinha( tamanho, t.formLinha, t.cor);
	setColor(15);//letras brancas e fundo preto
	pularLinha();
	
}

void mostrarErro( int numErro ){
	pularLinha();
	mostrarTitulo( titulos[numErro] );
	congelar();
	limparTela();
	
}

int lerInt(char *msg){
	int numero;
	printf("%s", msg);
	scanf("%i", &numero);
	fflush(stdin);
	return numero;	
}

double lerDouble(char *msg){
	double numero;
	printf("%s", msg);
	scanf("%lf", &numero);
	fflush(stdin);
	return numero;
}

char lerChar(char *msg){
	char caract;
	printf("%s", msg);
	caract = getchar();
	return caract;
}

int mostrarOpcs(char *mat[], int tam){
	int c;	
	int numero;
	char num[50];
	int index_opc   =  2;
	int index_erro  = 14;
	
	while (true){
		limparTela();
		mostrarTitulo( titulos[index_opc] );
		pularLinha();
		
		for (c=0; c<=(tam-1); c++){
			printf(" [ %d ] - %s\n", c+1, mat[c]);
		}
		
		desenharLinha( linha, '-', 15);
		numero = lerInt(" [ Escolha sua opção ] : " );
		
		if ( numero < 1 || numero > tam){
			pularLinha();
			mostrarTitulo( titulos[index_erro] );
			congelar();
		}
		
		else{
			return numero;
			break;
		}
	}
}

int getLogin(){
	
	//DECLARAÇÕES 
	char usuario[20];
	char   senha[20];
	
	int index_log = 1;
	int index_verLog = 11;
		
	//LÓGICA DO LOGIN ------------------------------------>
	mostrarTitulo( titulos[index_log] );
	printf(" [ USUÁRIO ]  : ");
	gets( usuario );
	fflush(stdin);
	printf(" [ SENHA   ]  : ");
	gets( senha );
	fflush(stdin);
	//---------------------------------------------------->
	
	pularLinha();
	mostrarTitulo( titulos[index_verLog] );
	congelar(                                           );
	limparTela(                                         );
	
	return 0;
}

int mostrarDados(CADASTRO c){
	int index_dadCad = 4;
	limparTela(                                                                                         );
	mostrarTitulo( titulos[index_dadCad] );
	printf(" [ NOME                ]    |    [ %44s ] \n",        c.nome                                );
	printf(" [ CPF                 ]    |    [ %44.0f ] \n",        c.cpf                               );
	printf(" [ TELEFONE            ]    |    [ %44.0f ] \n",        c.tel                               );
	printf(" [ RUA                 ]    |    [ %44s ] \n",        c.rua                                 );
	printf(" [ NÚMERO              ]    |    [ %44i ] \n",        c.num                                 );
	printf(" [ BAIRRO              ]    |    [ %44s ] \n",        c.bairro                              );
	printf(" [ CIDADE              ]    |    [ %44s ] \n",        c.cidade                              );
	printf(" [ ESTADO              ]    |    [ %44s ] \n",        c.estado                              );
	printf(" [ CEP                 ]    |    [ %44.0f ] \n",        c.cep                               );
	printf(" [ DATA-DE-NASCIMENTO  ]    |    [%35s%2i/%2i/%4i ] \n", " ", c.nasc.d, c.nasc.m, c.nasc.a  ); 
	printf(" [ EMAIL               ]    |    [ %44s ] \n", c.email                                      );
	printf(" [ DATA-DE-DIAGNÓSTICO ]    |    [%35s%2i/%2i/%4i ] \n", " ", c.diag.d, c.diag.m, c.diag.a  );
	printf(" [ COMORBIDADE         ]    |    [ %44s ] \n", c.comorb                                     );
	pausar(                                                                                             );
	return 0; 
}

void desenharLinhaFile( FILE *a){
	int c;
	fprintf(a, "+");
	for (c = 1; c<=linha; c++){
		fprintf(a, "=");
	}
	fprintf(a, "+\n");
}

int salvarCad(CADASTRO c){
	int index_dadCad = 12;
	FILE *a;
	mostrarTitulo( titulos[ index_dadCad ]                                           );
	congelar(                                                                        );
	a = fopen("Cadastro_Pacientes.txt", "a"                                          );
	fprintf(a, "\n"                                                                  );
	fprintf(a, " [ NOME                ] => %s\n", c.nome                            );
	fprintf(a, " [ CPF                 ] => %.0f\n", c.cpf                           );
	fprintf(a, " [ TELEFONE            ] => %.0f\n", c.cpf                           );
	fprintf(a, " [ RUA                 ] => %s\n", c.rua                             );
	fprintf(a, " [ NÚMERO              ] => %i\n", c.num                             );
	fprintf(a, " [ BAIRRO              ] => %s\n", c.bairro                          );
	fprintf(a, " [ CIDADE              ] => %s\n", c.cidade                          );
	fprintf(a, " [ ESTADO              ] => %s\n", c.estado                          );
	fprintf(a, " [ CEP                 ] => %.0f\n", c.cep                           );
	fprintf(a, " [ DATA-DE-NASCIMENTO  ] => %i/%i/%i\n", c.nasc.d, c.nasc.m, c.nasc.a);
	fprintf(a, " [ EMAIL               ] => %s\n",                            c.email);
	fprintf(a, " [ DATA-DE-DIAGNÓSTICO ] => %i/%i/%i\n", c.diag.d, c.diag.m, c.diag.a);
	fprintf(a, " [ COMORBIDADE         ] => %s\n",                           c.comorb);
	desenharLinhaFile( a );
	fclose(a                                                                         );
	return 0;
}

int salvarPacientesGrupRisc(int idade, int cep){
	FILE *a;
	a = fopen("Cadastro_Grupo_Risco.txt", "a"    );
	fprintf(a, "\n"                               );
	fprintf(a, " [ CEP   ] => %d\n",           cep);
	fprintf(a, " [ IDADE ] => %d\n",         idade);
	desenharLinhaFile( a );
	fclose(a                                      );
	return 0;
}

CADASTRO cadastrar(){
	CADASTRO c;
	char opc;
	int cont;
	limparTela();
	
	int index_cad   =  3;
	int index_end   =  6;
	int index_com   = 10;
	int index_email =  7;
	int index_dtn   =  8;
	int index_dtd   =  9;
	
	//parte do cadastro
	mostrarTitulo( titulos[index_cad] );
	printf(" [ NOME     ]  : ");
	gets( c.nome );
	fflush(stdin);
	c.cpf    = lerDouble(" [ CPF      ]  : ");
	c.tel    = lerDouble(" [ TELEFONE ]  : ");
	
	limparTela();
	
	//parte de cadastro do endereço
	mostrarTitulo( titulos[index_end] );
	pularLinha();
	printf(" [ RUA    ]  : ");
	gets( c.rua );
	fflush(stdin);
	c.num    = lerInt   (" [ NÚMERO ]  : ");
	printf(" [ BAIRRO ]  : ");
	gets( c.bairro );
	fflush(stdin);
	printf(" [ CIDADE ]  : ");
	gets( c.cidade );
	fflush(stdin);
	printf(" [ ESTADO ]  : ");
	gets( c.estado );
	fflush(stdin);
	c.cep    = lerDouble(" [ CEP    ]  : ");
	
	limparTela();
	
	//parte da data de nascimento
	mostrarTitulo( titulos[ index_dtn ] );
	pularLinha();
	c.nasc.d = lerInt(" [ DIA ]  : " );
	c.nasc.m = lerInt(" [ MÊS ]  : " );
	c.nasc.a = lerInt(" [ ANO ]  : " );
	
	limparTela();
	
	//parte do email
	mostrarTitulo( titulos[ index_email ] );
	pularLinha();
	printf("[ DIGITE \"none\" SE NÃO TIVER NENHUM ] : ");
	gets( c.email );
	fflush(stdin);
	
	limparTela();
	
	//parte da data de diagnóstico
	mostrarTitulo( titulos[ index_dtd ] );
	pularLinha();
	c.diag.d = lerInt(" [ DIA ]  : " );
	c.diag.m = lerInt(" [ MÊS ]  : " );
	c.diag.a = lerInt(" [ ANO ]  : " );
	
	limparTela();
	
	mostrarTitulo( titulos[ index_com ] );
	printf(" [ DIGITE \"none\" SE NÃO TIVER NENHUMA ] : ");
	gets( c.comorb );
	fflush(stdin);
	
	if ( c.comorb != "none" || c.comorb != "NONE"){
		
		c.temComorbidade = true;
	}
	else{
		c.temComorbidade = false;
	}

	return c;
}

int consultarArquivos( char *txt, int index_arq ){
	
	FILE *a;
	int index_erro   = 15;
	char linha[100];
	
	a = fopen( txt, "r");
				
	if ( a == NULL ){
		mostrarErro( index_erro );
		return 0;
	}
	
	limparTela();
	mostrarTitulo( titulos[index_arq] );
	
	while ( fgets( linha, 100, a) != NULL ){
		printf( "%s", linha );
	}
	
	pularLinha();
	pausar();
	
	fclose(a);
}

int consultar(){
	int opc;
	char *menuOpcs[] = { "CONSULTAR CADASTRADOS", "CONSULTAR GRUPO DE RISCO", "VOLTAR"};
	bool run = true;
	
	while (run){
		opc = mostrarOpcs( menuOpcs, 3);
		
		switch( opc ){
			case 1:
				consultarArquivos("Cadastro_Pacientes.txt", 16);
				break;
			case 2:
				consultarArquivos( "Cadastro_Grupo_Risco.txt", 17);
				break;
			case 3:
				run = false;
				break;
		}
	}
	
	limparTela();
	
	return 0;
}

int sair(){
	int index_sair = 5;
	limparTela();
	//FINAL DO PROGRAMA------------------------------------>
	mostrarTitulo( titulos[ index_sair ] );
	//----------------------------------------------------->
	return 0;
}
