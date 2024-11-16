#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
    char id;
    float x, y;
    char rua1[50];
    char rua2[50];
} Ponto;

typedef struct {
    char origem;
    char destino;
    char nomeRua[50];
    float distancia;
} Aresta;

typedef struct {
    int numPontos;
    int numArestas;
    Ponto* pontos;
    Aresta* arestas;
} Grafo;


Grafo* inicio_grafo(const char* pontosBin, const char* vizinhosBin);
void listar_pontos(Grafo* grafo);
void dijkstra(Grafo* grafo, char origem, char destino);
void instrucoes(Grafo* grafo, int* anterior, int destinoIdx, int origemIdx);

#endif
