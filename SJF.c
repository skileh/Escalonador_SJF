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


void inserir_No_Inicio (NO **p_Raiz, int p_temp, char p_nom[4]){
    if(*p_Raiz != NULL && (*p_Raiz)->p_cabeca == 1 ){
        (*p_Raiz)->p_tempo=p_temp;
        (*p_Raiz)->p_tempoChegada=0;
        (*p_Raiz)->p_tempoSaida=p_temp;

        strcpy((*p_Raiz)->p_nome,p_nom);
        (*p_Raiz)->p_cabeca = 0;
        printf("primeira insersão apos remocão");
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

int remover_No_Fim( NO **p_Raiz){
	int aux;
    if(*p_Raiz == NULL) printf("\nA lista ja esta vazia");
    else if((*p_Raiz)->p_prox == NULL){
        (*p_Raiz)->p_cabeca = 1;
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
        free(p_atual);
    }
}

void imprime (NO **le, int n) {
    NO *p;
    printf("Processador_%d",n);
    for (p = *le; p != NULL; p = p->p_prox)
       printf ("\n%s;%d;%d",p->p_nome, p->p_tempoChegada,p->p_tempoSaida);

  	printf("\n");
}

void organizaLista(Tarefa *lista, int tam){
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

    //print da lista ordenada
    for (j = 0; j < tam; j++) {
        printf("#%s, %d  # ", lista[j].nome, lista[j].tempo);
    }
    printf("\n");
}

void escalonador(NO **lista,int qnt_tarefa, int qnt_processador){
	Tarefa *taref;
	taref = (Tarefa*)malloc((qnt_tarefa)*sizeof(Tarefa));

	strcpy(taref[0].nome,"a1");
	taref[0].tempo=5;
	strcpy(taref[1].nome,"a2");
	taref[1].tempo=1;
	strcpy(taref[2].nome,"a3");
	taref[2].tempo=10;
	strcpy(taref[3].nome,"b1");
	taref[3].tempo=10;
	strcpy(taref[4].nome,"b2");
	taref[4].tempo=3;
	strcpy(taref[5].nome,"b3");
	taref[5].tempo=7;
	strcpy(taref[6].nome,"b4");
	taref[6].tempo=8;
	strcpy(taref[7].nome,"c1");
	taref[7].tempo=8;
	strcpy(taref[8].nome,"c2");
	taref[8].tempo=2;
	organizaLista(taref,qnt_tarefa);

    for(int i=0;i<qnt_tarefa;){
    	for(int j=0;j<2;j++){
    		inserir_No_Inicio((&lista[j]),taref[i].tempo,taref[i].nome);
    		i++;
    		if(i==qnt_tarefa) break;
    	}
    }

    printf("\n");
    for(int i=0;i<2;i++){
    	printf("\n");
    	imprime(&lista[i],i+1);
    }
}




void main(){
	NO **lista;
	int qnt_tarefa,qnt_processador;
	qnt_tarefa=9;
	qnt_processador=2;
	lista=(NO**)malloc((qnt_processador)*sizeof(NO));
    criar_Lista(*(&lista));
    escalonador(lista,qnt_tarefa,qnt_processador);
	//imprima(f);
	//printf(listinha);


}
