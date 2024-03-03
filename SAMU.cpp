#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <time.h>
#define infinito 99999

const int SAMU = 83, H1 = 40, H2 = 19, H3 = 82;

typedef struct vertice{
    int valor;
    struct vertice *prox;
}Vertice;

typedef struct caminho{
    Vertice* inicio;
}Caminho;

typedef struct grafo{
    int vertice;
    int** matrizAdj;
    int* grau;
    Caminho** matrizCaminho;
}Grafo;

Grafo* Cria_Grafo(int nVert){
    Vertice* InserirVertice(int dest);

    Grafo* grafo_novo = (Grafo*) malloc(sizeof(Grafo));

    if(grafo_novo != NULL){
        grafo_novo->vertice = nVert;
        grafo_novo->grau = (int*) calloc (nVert, sizeof(int));
        grafo_novo->matrizAdj = (int**) malloc(nVert * sizeof(int*));
        grafo_novo->matrizCaminho = (Caminho**) malloc(nVert * sizeof(Caminho*));

        for(int i=0; i<nVert; i++){
            grafo_novo->matrizAdj[i] = (int*) calloc(nVert, sizeof(int));
        }

        for(int i=0; i<nVert; i++){
            grafo_novo->matrizCaminho[i] = (Caminho*) malloc(nVert * sizeof(Caminho));
        }

        for(int i=0; i<nVert; i++){
            for(int j=0; j<nVert; j++){
                if(i != j){
                    grafo_novo->matrizAdj[i][j] = infinito;
                }
            }
        }
        
        for(int i=0; i<nVert; i++){
            for(int j=0; j<nVert; j++){
                grafo_novo->matrizCaminho[i][j].inicio = InserirVertice(i);
            }
        }

    }
    printf("grafo criado\n");
    return grafo_novo;
}

void Libera_Grafo(Grafo* grafo){
    if(grafo != NULL){
        free(grafo->grau);

        for(int i=0; i<grafo->vertice; i++){
            free(grafo->matrizAdj[i]);
            free(grafo->matrizCaminho[i]);
        }
        free(grafo->matrizAdj);
        free(grafo->matrizCaminho);

        free(grafo);
    }
}

Vertice* InserirVertice(int dest){
    if(Vertice* novo = (Vertice*) malloc(sizeof(Vertice))){

        novo->prox = NULL;
        novo->valor = dest;

        return novo;
    }

    return NULL;
}

int InsereArco(Grafo* grafo, int orig, int dest, float peso){
    void Mostra_Caminho(Vertice* caminho);

    if(grafo == NULL) return 0;
    if(orig < 0 || orig > grafo->vertice) return 0;
    if(dest < 0 || dest > grafo->vertice) return 0;

    grafo->matrizAdj[orig][dest] = peso;

    grafo->grau[orig]++;

    if(peso == 0){
        free(grafo->matrizCaminho[orig][dest].inicio = NULL);
    
    }else{
        free(grafo->matrizCaminho[orig][dest].inicio->prox);
        Vertice* novo = InserirVertice(dest);
        novo->prox = NULL;
        grafo->matrizCaminho[orig][dest].inicio->prox = novo;
        grafo->matrizCaminho[orig][dest].inicio->prox->prox = NULL;

    }

    return 1;
}

Vertice* InserirVertCamin(Vertice* orig, Vertice* dest){
    Vertice* aux1 = dest;

    while(aux1->prox != NULL){
        aux1 = aux1->prox;
    }

    aux1->prox = InserirVertice(orig->valor);
    aux1->prox->prox = NULL;

    return dest;
}

Vertice* AtualizarCaminho(Vertice* vert1, Vertice* vert2, Vertice* vert3){
    void Mostra_Caminho(Vertice* caminho);

    Vertice *aux1 = vert1->prox;
    vert3->prox = NULL;

    while(aux1 != NULL){
        vert3 = InserirVertCamin(aux1, vert3);
        aux1 = aux1->prox;
    }

    aux1 = vert3->prox;
    Vertice *aux2 = vert2;

    while(aux1->valor != aux2->valor){
        aux1 = aux1->prox;

        if(aux1 == NULL){
            break;
        }
    }

    aux1->prox = aux2->prox;

    return vert3;
}

Grafo* AlgoritmoFW(Grafo* g, int k){
    void Mostra_Grafo(Grafo *grafo);
    void Mostra_Caminho(Vertice* caminho);

    if(k < g->vertice){
        for(int i=0; i<g->vertice; i++){
            for(int j=0; j<g->vertice; j++){
                if(i != k && j != k && i != j){ 
                    if(g->matrizAdj[i][j] > g->matrizAdj[i][k] + g->matrizAdj[k][j]){ 
                        g->matrizAdj[i][j] = g->matrizAdj[i][k] + g->matrizAdj[k][j];

                        g->matrizCaminho[i][j].inicio = AtualizarCaminho(g->matrizCaminho[i][k].inicio, g->matrizCaminho[k][j].inicio, g->matrizCaminho[i][j].inicio);
                    }
                }
            }
        }

        g = AlgoritmoFW(g, k+1);
    }else{
        return g;
    }

    return g;
}

void Mostra_Grafo(Grafo *grafo){ 
    for(int i=0; i<grafo->vertice; i++){
        printf("\t%d", i);
    }
    printf("\n");

    for(int i=0; i<grafo->vertice; i++){
        printf("%d", i);
        for(int j=0; j<grafo->vertice; j++){
            printf("\t%d", grafo->matrizAdj[i][j]);
        }
        printf("\n\n");
    }
}

Grafo* Ler_Grafo(Grafo* grafo){
    FILE* grafoTXT;

    grafoTXT = fopen("Dist.txt", "r");

    int linha = 0, coluna = 0, peso;
    char *linhaTxt, *token;
    char text[1000];

    while(!feof(grafoTXT)){
        linhaTxt = fgets(text, 1000, grafoTXT);

        token = strtok(text, "\t");
        while (token != NULL) {
            peso = atoi(token);

            InsereArco(grafo, linha, coluna, peso);

            coluna++;
            token = strtok(NULL, "\t");
        }

        free(token);
        linha++;
        coluna = 0;
    }


    fclose(grafoTXT);
    return grafo;
}

int sorteio(){
    int num;
    
    srand(time(NULL));
    num = rand() % 110;

    return num;
}

int menor(Grafo* g, int vert){
    if(g->matrizAdj[vert][H1] < g->matrizAdj[vert][H2] && g->matrizAdj[vert][H1] < g->matrizAdj[vert][H2]){
        return H1;
    }else if(g->matrizAdj[vert][H2] < g->matrizAdj[vert][H1] && g->matrizAdj[vert][H2] < g->matrizAdj[vert][H3]){
        return H2;
    }else{
        return H3;
    }
}

void Menu(Grafo* grafo){
    void Mostra_Caminho(Vertice* caminho);
    int opc = 0;
    int vert1, vert2;
    int hosp1, hosp2;
    
    while(opc != 5){
        system("cls");
        printf("======================\n");
        printf("         MENU         \n");
        printf("======================\n");
        printf("1 - Sortear dois locais\n");
        printf("2 - Calcular distancia(Ambulancia - Acidente)\n");
        printf("3 - Calcular distancia(Acidente - Hospital)\n");
        printf("4 - Calcular distancia total\n");
        printf("5 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opc);

        switch (opc)
        {
        case 1:
            system("cls");
            printf("====================\n");
            printf("       Sorteio      \n");
            printf("====================\n");
            vert1 = sorteio();
            vert2 = sorteio();

            while (vert1 == vert2 || vert1 == 40 || vert1 == 19 || vert1 == 82 || vert1 == 83 || vert2 == 40 || vert2 == 19 || vert2 == 82 || vert2 == 83)
            {
                vert2 = sorteio();
            }
            
            printf("Acidente nos locais %d e %d\n", vert1, vert2);
            system("pause");
            break;

        case 2:
            system("cls");
            printf("==================================\n");
            printf("       Ambulancia - Acidente      \n");
            printf("==================================\n");
            printf("Acidente %d- %dm\n", vert1, grafo->matrizAdj[vert1][SAMU]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[SAMU][vert1].inicio);
            printf("Fim\n");
            printf("Acidente %d- %dm\n", vert2, grafo->matrizAdj[vert2][SAMU]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[SAMU][vert2].inicio);
            printf("Fim\n");
            system("pause");
            break;

        case 3:
            system("cls");
            printf("==================================\n");
            printf("        Acidente - Hospital       \n");
            printf("==================================\n");
            
            hosp1 = menor(grafo, vert1);
            hosp2 = menor(grafo, vert2);

            printf("Os hospitais mais proximos:\n");
            printf("Acidente %d- %dm\n", vert1, grafo->matrizAdj[vert1][hosp1]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[vert1][hosp1].inicio);
            printf("Fim\n");
            printf("Acidente %d- %dm\n", vert2, grafo->matrizAdj[vert2][hosp2]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[vert2][hosp2].inicio);
            printf("Fim\n");
            system("pause");

            break;
        case 4:
            system("cls");
            printf("==================================\n");
            printf("               TOTAL              \n");
            printf("==================================\n");

            printf("Acidente %d -- Hospital:\n", vert1);
            printf("%dm\n", grafo->matrizAdj[vert1][SAMU]+grafo->matrizAdj[vert1][hosp1]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[SAMU][vert1].inicio);
            Mostra_Caminho(grafo->matrizCaminho[vert1][hosp1].inicio);
            printf("Fim\n");

            printf("Acidente %d -- Hospital:\n", vert2);
            printf("%dm\n", grafo->matrizAdj[vert2][SAMU]+grafo->matrizAdj[vert2][hosp2]);
            printf("Rota: ");
            Mostra_Caminho(grafo->matrizCaminho[SAMU][vert2].inicio);
            Mostra_Caminho(grafo->matrizCaminho[vert2][hosp2].inicio);
            printf("Fim\n");
            system("pause");

            break;
        default:
            break;
        }
    }
}

void Mostra_Caminho(Vertice* caminho){
    Vertice* aux = caminho;

    while(aux != NULL){

        if(aux->valor == 83){
            printf("SAMU - ");
        }else if(aux->valor == 40){
            printf("Hospital 1 - ");
        }else if(aux->valor == 19){
            printf("Hospital 2 - ");
        }else if(aux->valor == 82){
            printf("Hospital 3 - ");
        }else{
            printf("%d - ", aux->valor);
        }

        aux = aux->prox;
    }
}

Grafo* grafoReal, *grafoMenor;

int main(){
    grafoMenor = Cria_Grafo(110);
    grafoMenor = Ler_Grafo(grafoMenor);
    grafoMenor = AlgoritmoFW(grafoMenor, 0);
    Menu(grafoMenor);

    Libera_Grafo(grafoReal);
    return 0;
}
