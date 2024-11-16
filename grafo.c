#include "grafo.h"

#define inf 9999999


float dist_ec(Ponto* p1, Ponto* p2) {
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

Grafo* inicio_grafo(const char* pontosBin, const char* vizinhosBin) 
{
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (!grafo) {
        printf("Erro ao alocar memória para o grafo.\n");
        exit(1);
    }

    FILE* filePontos = fopen(pontosBin, "rb");
    if (!filePontos) {
        printf("Erro ao abrir arquivo de pontos.\n");
        free(grafo);
        exit(1);
    }

    fread(&grafo->numPontos, sizeof(int), 1, filePontos);
    grafo->pontos = (Ponto*)malloc(grafo->numPontos * sizeof(Ponto));
    fread(grafo->pontos, sizeof(Ponto), grafo->numPontos, filePontos);
    fclose(filePontos);

    FILE* fileVizinhos = fopen(vizinhosBin, "rb");
    if (!fileVizinhos) {
        printf("Erro ao abrir arquivo de vizinhos.\n");
        free(grafo->pontos);
        free(grafo);
        exit(1);
    }

    fread(&grafo->numArestas, sizeof(int), 1, fileVizinhos);
    grafo->arestas = (Aresta*)malloc(grafo->numArestas * sizeof(Aresta));
    fread(grafo->arestas, sizeof(Aresta), grafo->numArestas, fileVizinhos);
    fclose(fileVizinhos);

    for (int i = 0; i < grafo->numArestas; i++) {
        Aresta* aresta = &grafo->arestas[i];
        Ponto* p1 = &grafo->pontos[aresta->origem - 'A'];
        Ponto* p2 = &grafo->pontos[aresta->destino - 'A'];
        aresta->distancia = dist_ec(p1, p2);
    }

    return grafo;
}

void listar_pontos(Grafo* grafo) {
    printf("Pontos disponíveis:\n");
    for (int i = 0; i < grafo->numPontos; i++) {
        printf("%c - %s e %s\n", grafo->pontos[i].id, grafo->pontos[i].rua1, grafo->pontos[i].rua2);
    }
}

void dijkstra(Grafo* grafo, char origem, char destino) {
    int n = grafo->numPontos;
    float* dist = (float*)malloc(n * sizeof(float));
    int* anterior = (int*)malloc(n * sizeof(int));
    int* visitado = (int*)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++) {
        dist[i] = INFINITY;
        anterior[i] = -1;
    }

    int origemIdx = origem - 'A';
    dist[origemIdx] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INFINITY) break;

        visitado[u] = 1;
        for (int j = 0; j < grafo->numArestas; j++) {
            Aresta* aresta = &grafo->arestas[j];
            if (aresta->origem - 'A' == u) {
                int v = aresta->destino - 'A';
                float alt = dist[u] + aresta->distancia;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    anterior[v] = u;
                }
            }
        }
    }

    int destinoIdx = destino - 'A';
    if (dist[destinoIdx] == INFINITY) {
        printf("Não há caminho entre %c e %c.\n", origem, destino);
    } else {
        instrucoes(grafo, anterior, destinoIdx, origemIdx);
    }

    free(dist);
    free(anterior);
    free(visitado);
}


void instrucoes(Grafo* grafo, int* anterior, int destinoIdx, int origemIdx) {
    if (anterior[destinoIdx] == -1) {
        printf("Não há caminho entre %c e %c.\n", grafo->pontos[origemIdx].id, grafo->pontos[destinoIdx].id);
        return;
    }

    printf("Ponto de origem: %c\n", grafo->pontos[origemIdx].id);
    printf("Ponto de destino: %c\n", grafo->pontos[destinoIdx].id);
    printf("Para realizar o percurso entre o ponto %c (Rua %s com Rua %s) e o ponto %c (Rua %s com Rua %s), faca os seguintes movimentos:\n",
           grafo->pontos[origemIdx].id, grafo->pontos[origemIdx].rua1, grafo->pontos[origemIdx].rua2,
           grafo->pontos[destinoIdx].id, grafo->pontos[destinoIdx].rua1, grafo->pontos[destinoIdx].rua2);

    int atual = destinoIdx;
    int passos = 1;

   
    while (atual != origemIdx) {
        int anteriorIdx = anterior[atual];
        Aresta* aresta = NULL;

        for (int i = 0; i < grafo->numArestas; i++) {
            if (grafo->arestas[i].origem == grafo->pontos[anteriorIdx].id &&
                grafo->arestas[i].destino == grafo->pontos[atual].id) {
                aresta = &grafo->arestas[i];
                break;
            }
        }

        if (aresta) {
           
            if (strncmp(grafo->pontos[atual].rua1, aresta->nomeRua,4 ) == 0) {
               
                printf("(%d) Siga em frente pela Rua %s ate o cruzamento com a Rua %s\n", passos,
                       grafo->pontos[atual].rua1, grafo->pontos[atual].rua2);
            } else {
                
                if (strncmp(grafo->pontos[atual].rua2, aresta->nomeRua,4 ) == 0) {
                    printf("(%d) No cruzamento da Rua %s com a Rua %s, vire a direita\n", passos,
                           grafo->pontos[atual].rua1, grafo->pontos[atual].rua2);
                } else {
                    printf("(%d) No cruzamento da Rua %s com a Rua %s, vire a esquerda\n", passos,
                           grafo->pontos[atual].rua1, grafo->pontos[atual].rua2);
                }
            }
            passos++;
        }
        atual = anteriorIdx;
    }

    printf("Boa viagem!\n");
}

