#include "grafo.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Uso: %s <pontos.bin> <vizinhos.bin> <origem> <destino>\n", argv[0]);
        return 1;
    }

    const char* pontosBin = argv[1];
    const char* vizinhosBin = argv[2];
    char origem = argv[3][0];
    char destino = argv[4][0];

    Grafo* grafo = inicio_grafo(pontosBin, vizinhosBin);
    listar_pontos(grafo);
    dijkstra(grafo, origem, destino);

    free(grafo->pontos);
    free(grafo->arestas);
    free(grafo);

    return 0;
}

