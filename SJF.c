#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct tarefa{
	int tempo;
	char nome[4];
}Tarefa;

typedef struct No{
    int p_cabeca;
    int p_tempo;
    int p_tempoChegada;
    int p_tempoSaida;
    char p_nome[4];
    struct No *p_prox;
}NO;

void criar_Lista(NO **p_Raiz){
    *p_Raiz = NULL;
}

//Insere na fila
void inserir_No_Inicio (NO **p_Raiz, int p_temp, char p_nom[4]){
    if(*p_Raiz != NULL && (*p_Raiz)->p_cabeca == 1 ){
        (*p_Raiz)->p_tempo=p_temp;
        (*p_Raiz)->p_tempoChegada=0;
        (*p_Raiz)->p_tempoSaida=p_temp;

        strcpy((*p_Raiz)->p_nome,p_nom);
        (*p_Raiz)->p_cabeca = 0;
     //   printf("primeira insersão apos remocão");
    }
    else{
        NO *p_Novo;
             /** Alocação dinâmica da memoria */
        p_Novo = (NO *) malloc(sizeof(struct No));  
        if( p_Novo == NULL ){
            printf( "Falta Memoria\n");
            return;
        }
         
        p_Novo->p_tempo = p_temp;
        if(*p_Raiz!=NULL){
       		p_Novo->p_tempoChegada = (*p_Raiz)->p_tempoSaida;
        	p_Novo->p_tempoSaida = p_Novo->p_tempoChegada+p_temp;
    	}
    	else{
    		p_Novo->p_tempoChegada = 0;
        	p_Novo->p_tempoSaida = p_temp;
    	}
     	strcpy(p_Novo->p_nome,p_nom);
        p_Novo->p_prox = *p_Raiz;
        *p_Raiz = p_Novo;
    }
}

//Remove todos elementos de uma lista recursivamente;
// !! ANTES da remoção os elementos são printados na tela !!
//Esta função percorre a fila de cima a baixo, removendo 
//sempre o primeiro elemento que foi inserido
//ou seja, a cada remoção, é necessário percorrer toda 
//fila. A execução acaba quando o ultimo elemento é removido (a cabeça é removida).
int remover_No_Fim( NO **p_Raiz, FILE *arq1, FILE *arq2){
	int aux;
    int i = 0;

    if(*p_Raiz == NULL){}//printf("\nA lista ja esta vazia");
    else if((*p_Raiz)->p_prox == NULL){
        (*p_Raiz)->p_cabeca = 1;
        fprintf(arq1,"\n%s;%d;%d\n\n",(*p_Raiz)->p_nome, (*p_Raiz)->p_tempoChegada,(*p_Raiz)->p_tempoSaida);
        fprintf(arq2,"\n%s;%d;%d\n\n",(*p_Raiz)->p_nome, (*p_Raiz)->p_tempoChegada,(*p_Raiz)->p_tempoSaida);
        free((*p_Raiz));
    }
    else{
    	int val;
        NO *p_atual, *p_anterior ;
        p_atual = *p_Raiz;
        while(p_atual->p_prox != NULL){
            p_anterior = p_atual ;
            p_atual    = p_atual->p_prox;
        }
        p_anterior->p_prox = NULL;
        fprintf(arq1,"\n%s;%d;%d",p_atual->p_nome, p_atual->p_tempoChegada,p_atual->p_tempoSaida);
        fprintf(arq2,"\n%s;%d;%d",p_atual->p_nome, p_atual->p_tempoChegada,p_atual->p_tempoSaida);
        free(p_atual);
        remover_No_Fim(*(&p_Raiz),arq1,arq2);
    }
}


//função de checagem de lista, imprime todos elementos contidos em uma lista
// como parametro é esperado uma lista..



//Esta função organiza um vetor em ordem crescente ou decrescente;
//Como parametro recebe o vetor, seu tamanho e o valor 0 caso seja crescente
// e 1 caso decrescente.
//Para ordenação de elementos é utilizado o algoritmo bubble sort.
void SJF(Tarefa *lista, int tam){
	int k, j;
	Tarefa aux;
	    for (k = tam - 1; k > 0; k--) {
	        for (j = 0; j < k; j++) {
	            if (lista[j].tempo > lista[j + 1].tempo) {
	                aux          = lista[j];
	                lista[j]     = lista[j + 1];
	                lista[j + 1] = aux;
	            }
	        }
	    }
}


void SJF_Oposto(Tarefa *lista, int tam){    
	int k, j;
    Tarefa aux;
		for (k = tam - 1; k > 0; k--) {
	        for (j = 0; j < k; j++) {
	            if (lista[j].tempo < lista[j + 1].tempo) {
	                aux = lista[j];
	                lista[j] = lista[j + 1];
	                lista[j + 1] = aux;
	            }
	        }
	    }
}


//Esta etapa consiste em receber os dados de entrada de um TXT e armazenalos em uma lista
//após a criação da lista, os dados são ordenados crescente e decrecentemente.
//logo os dados são separados em seus respectiveis processadores, onde será contabilizado
//seu tempo de chegada e saida, utilizando uma FILA
//por ultimo, os dados são desalocados da fila de cada processador e salvos em um TXT

//como parametro, recebe fila de processador, quantidade de processadores, e um valor de checagem pra cada SJF
void escalonador1(NO **lista, int qnt_processador, FILE *f, FILE *arq1,FILE *arq2){
	Tarefa *taref;
	
	//TAREFA é *taref DE ELEMENTOS 
	//FAZER A LEITURA DO TXT AQUI e armazenar no vetor
    int i; 

    f = fopen("tarefas.txt", "r");
    if (f == NULL){
        return;
    }

    arq1 = fopen("menor_primeiro.txt", "w");
    if(arq1 == NULL){
        printf("ERRO!");
        exit(1);
    }    
 

	int qnt_tarefa=9; // quantidade de tarefas
	taref = (Tarefa*)malloc((qnt_tarefa)*sizeof(Tarefa));

    for (i=0; i < qnt_tarefa; i++){
        fscanf(f, "%s %d",taref[i].nome, &taref[i].tempo);
    }
    
	//AS TAREFAS SÃO ORGANIZADAS, ou melhor, ORDENADAS
	SJF(taref,qnt_tarefa);

	//Nesta etapa as tarefas são distribuidas para o processador
	//Elas são enviadas ao processador, mas antes, é feito uma consulta para ver qual tarefa 
	// vai acabar primeiro.
	//são inseridas no processador de forma crescente, em ambos casos.
	//Esse primeiro laço insere a PRIMEIRA tarefa em cada processador 
	for(int j=0;j<qnt_processador;j++){
    	inserir_No_Inicio((&lista[j]),taref[j].tempo,taref[j].nome);
	}
	//Já neste, segue o padrão de inserção crescente, ou seja, verifica que tarefa vai acabar primeiro...
	int aux=999999,auxj;
    for(int i=qnt_processador;i<qnt_tarefa;i++){
    	for(int j=0;j<qnt_processador;j++){
    		if((lista[j])->p_tempoSaida<aux ){
    			aux=(lista[j])->p_tempoSaida;
    			auxj=j;
    		}    		
    		if(i==qnt_tarefa){break;}
    	}
    	inserir_No_Inicio((&lista[auxj]),taref[i].tempo,taref[i].nome);
    	aux=999999;
    }
    for(int i=0;i<2;i++){
    	fprintf(arq1,"Processador_%d",i+1);
        remover_No_Fim(&lista[i],arq1,arq2);
    	printf("\n");
    }
}

void escalonador2(NO **lista, int qnt_processador, FILE *f,FILE *arq1, FILE *arq2){
   Tarefa *taref;
    
    //TAREFA é *taref DE ELEMENTOS 
    //FAZER A LEITURA DO TXT AQUI e armazenar no vetor
    int i; 

    f = fopen("tarefas.txt", "r");
    if (f == NULL){
        return;
    }
    arq2 = fopen("maior_primeiro.txt", "w");
    if(arq2 == NULL){
        printf("ERRO!");
        exit(1);
    }    
    int qnt_tarefa=9; // quantidade de tarefas
    taref = (Tarefa*)malloc((qnt_tarefa)*sizeof(Tarefa));

    for (i=0; i < qnt_tarefa; i++){
        fscanf(f, "%s %d",taref[i].nome, &taref[i].tempo);
    }
    
    //AS TAREFAS SÃO ORGANIZADAS, ou melhor, ORDENADAS
    SJF_Oposto(taref,qnt_tarefa);

    //Nesta etapa as tarefas são distribuidas para o processador
    //Elas são enviadas ao processador, mas antes, é feito uma consulta para ver qual tarefa 
    // vai acabar primeiro.
    //são inseridas no processador de forma crescente, em ambos casos.
    //Esse primeiro laço insere a PRIMEIRA tarefa em cada processador 
    for(int j=0;j<qnt_processador;j++){
        inserir_No_Inicio((&lista[j]),taref[j].tempo,taref[j].nome);
    }
    //Já neste, segue o padrão de inserção crescente, ou seja, verifica que tarefa vai acabar primeiro...
    int aux=999999,auxj;
    for(int i=qnt_processador;i<qnt_tarefa;i++){
        for(int j=0;j<qnt_processador;j++){
            if((lista[j])->p_tempoSaida<aux ){
                aux=(lista[j])->p_tempoSaida;
                auxj=j;
            }           
            if(i==qnt_tarefa){break;}
        }
        inserir_No_Inicio((&lista[auxj]),taref[i].tempo,taref[i].nome);
        aux=999999;
    }
    for(int i=0;i<2;i++){
        fprintf(arq2,"Processador_%d",i+1);
        remover_No_Fim(&lista[i], arq1, arq2);
        printf("\n");
    }
}

void main(){
	NO **lista;
	int qnt_processador;

	printf("Insira a quantidade de processadores: ");
	scanf("%d", &qnt_processador);

	
    FILE *f;
    FILE *arq1;
    FILE *arq2;

    
    ///get(NOME DO ARQUIVO)
	//O NOME DO ARQUIVO DEVE SER PASSADO COMO PARAMETRO PRA FUNCAO ESCALONADOR
	//NELA O ARQUIVO SErÁ LIDO..

	//SJF
	lista=(NO**)malloc((qnt_processador)*sizeof(NO));
    criar_Lista(*(&lista));
    escalonador1(lista,qnt_processador,f,arq1,arq2);

    //SJF DECRESCENTE
    NO **lista2;
	lista2=(NO**)malloc((qnt_processador)*sizeof(NO));
    criar_Lista(*(&lista2));
    escalonador2(lista2,qnt_processador,f,arq1,arq2);


    free(lista);
    free(lista2);
    fclose(f);
    fclose(arq1);
    fclose(arq2);
	

}
