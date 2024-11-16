// grafo.h

#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_VIZINHOS 4
#define MAX_NOME_RUA 50

typedef struct {
    char id;
    float x, y;
    char rua1[MAX_NOME_RUA];
    char rua2[MAX_NOME_RUA];
} Ponto;

typedef struct {
    char origem;
    char destino;
    char nomeRua[MAX_NOME_RUA];
    float distancia;
} Aresta;

typedef struct {
    int numPontos;
    int numArestas;
    Ponto* pontos;
    Aresta* arestas;
} Grafo;

// Funções para manipulação do grafo
Grafo* inicializa_grafo(const char* pontosBin, const char* vizinhosBin);
void insere_arestas(Grafo* grafo);
void dijkstra(Grafo* grafo, char origem, char destino);
void gerar_relatorio(Grafo* grafo, char origem, char destino, const char* arquivoRelatorio);

#endif
