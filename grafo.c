#include "grafo.h"

#define inf 9999999

Grafo* inicializa_grafo(const char* pontosBin, const char* vizinhosBin) {
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
        printf("Distância mínima: %.2f\n", dist[destinoIdx]);
        exibir_instrucoes(grafo, anterior, destinoIdx, origemIdx);
    }

    free(dist);
    free(anterior);
    free(visitado);
}

void gerar_relatorio(Grafo* grafo, char origem, char destino, const char* arquivoRelatorio) {
    FILE* arquivo = fopen(arquivoRelatorio, "w");
    if (!arquivo) {
        printf("Erro ao criar o relatório.\n");
        return;
    }
    fprintf(arquivo, "Relatório de caminho entre %c e %c.\n", origem, destino);
    fclose(arquivo);
}


void exibir_instrucoes(Grafo* grafo, int* anterior, int destinoIdx, int origemIdx) {
    if (anterior[destinoIdx] == -1) {
        printf("Não há caminho entre %c e %c.\n", grafo->pontos[origemIdx].id, grafo->pontos[destinoIdx].id);
        return;
    }

    printf("Para realizar o percurso entre o ponto %c (%s com %s) e o ponto %c (%s com %s), faça os seguintes movimentos:\n",
           grafo->pontos[origemIdx].id, grafo->pontos[origemIdx].rua1, grafo->pontos[origemIdx].rua2,
           grafo->pontos[destinoIdx].id, grafo->pontos[destinoIdx].rua1, grafo->pontos[destinoIdx].rua2);

    // Reconstruir o caminho do destino até a origem
    int atual = destinoIdx;
    int passos = 1;

    while (atual != origemIdx) {
        int anteriorIdx = anterior[atual];
        Aresta* aresta = NULL;

        // Encontrar a aresta que conecta 'anteriorIdx' e 'atual'
        for (int i = 0; i < grafo->numArestas; i++) {
            if (grafo->arestas[i].origem == grafo->pontos[anteriorIdx].id &&
                grafo->arestas[i].destino == grafo->pontos[atual].id) {
                aresta = &grafo->arestas[i];
                break;
            }
        }

        if (aresta) {
            printf("(%d) Siga em frente pela %s até o cruzamento com a %s.\n",
                   passos, aresta->nomeRua, grafo->pontos[atual].rua1);
            passos++;
        }
        atual = anteriorIdx;
    }

    printf("Boa viagem!\n");
}
