#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grafo.h"

#define INFINITY 9999999

// Função para inicializar o grafo lendo dados dos arquivos binários
Grafo* inicializa_grafo(const char* pontosBin, const char* vizinhosBin) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (!grafo) {
        printf("Erro: Falha ao alocar memória para o grafo.\n");
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
    if (!grafo->pontos) {
        printf("Erro: Falha ao alocar memória para os pontos.\n");
        fclose(filePontos);
        free(grafo);
        exit(1);
    }
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
    if (!grafo->arestas) {
        printf("Erro: Falha ao alocar memória para as arestas.\n");
        fclose(fileVizinhos);
        free(grafo->pontos);
        free(grafo);
        exit(1);
    }
    fread(grafo->arestas, sizeof(Aresta), grafo->numArestas, fileVizinhos);
    fclose(fileVizinhos);

    return grafo;
}

// Função para calcular a distância entre dois pontos
float calcula_distancia(Ponto* p1, Ponto* p2) {
    return sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
}

//Função para listar todos os pontos disponíveis
void listar_pontos(Grafo* grafo) {
    printf("Pontos disponíveis:\n");
    for (int i = 0; i < grafo->numPontos; i++) {
        printf("%c - %s e %s\n", 'A' + i, grafo->pontos[i].rua1, grafo->pontos[i].rua2);
    }
}

// Função para solicitar um ponto de origem ou destino
char solicitar_ponto(char tipo, Grafo* grafo) {
    char ponto;
    int valido = 0;
    
    while (!valido) {
        printf("Digite um ponto de %s: ", tipo);
        scanf(" %c", &ponto);

        // Verifica se o ponto digitado é válido
        if (ponto >= 'A' && ponto < 'A' + grafo->numPontos) {
            valido = 1;
        } else {
            printf("Ponto inválido. Tente novamente.\n");
        }
    }
    
    return ponto;
}


// Implementação do Algoritmo de Dijkstra
void dijkstra(Grafo* grafo, char origem, char destino) {
    int n = grafo->numPontos;
    float* dist = (float*)malloc(n * sizeof(float));
    int* visitado = (int*)calloc(n, sizeof(int));
    int* anterior = (int*)malloc(n * sizeof(int));  // Array para armazenar os pontos anteriores
    if (!dist || !visitado || !anterior) {
        printf("Erro: Falha ao alocar memória para o algoritmo de Dijkstra.\n");
        free(dist);
        free(visitado);
        free(anterior);
        return;
    }

    for (int i = 0; i < n; i++) {
        dist[i] = INFINITY;
        anterior[i] = -1;  // Inicia com nenhum ponto anterior
    }

    int idxOrigem = origem - 'A'; // Considerando IDs como letras consecutivas (A, B, C...)
    dist[idxOrigem] = 0;

    for (int i = 0; i < n - 1; i++) {
        // Encontrar o próximo nó não visitado com menor distância
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INFINITY) break;

        visitado[u] = 1;

        // Atualizar as distâncias dos vizinhos
        for (int j = 0; j < grafo->numArestas; j++) {
            Aresta* aresta = &grafo->arestas[j];
            if (aresta->origem == 'A' + u) {
                int v = aresta->destino - 'A';
                if (!visitado[v] && dist[u] + aresta->distancia < dist[v]) {
                    dist[v] = dist[u] + aresta->distancia;
                    anterior[v] = u; // Armazena o índice do ponto anterior
                }
            }
        }
    }

    // Exibir o caminho com instruções de movimentação
    int idxDestino = destino - 'A';
    if (dist[idxDestino] == INFINITY) {
        printf("nao ha caminho entre %c e %c.\n", origem, destino);
    } else {
        printf("Menor distancia de %c a %c: %.2f\n", origem, destino, dist[idxDestino]);
        printf("Para realizar o percurso entre o ponto %c e o ponto %c, faca os seguintes movimentos:\n", origem, destino);
        // Código para reconstrução do caminho e exibição das instruções
    }

    free(dist);
    free(visitado);
    free(anterior);
}
