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

    Grafo* grafo = inicializa_grafo(pontosBin, vizinhosBin);
     listar_pontos(grafo);

    // Solicita o ponto de origem e destino
    char origem = solicitar_ponto("origem", grafo);
    char destino = solicitar_ponto("destino", grafo);
    
    dijkstra(grafo, origem, destino);
    gerar_relatorio(grafo, origem, destino, "relatorio.txt");

    printf("Relatório gerado com sucesso em relatorio.txt\n");

    free(grafo->pontos);
    free(grafo->arestas);
    free(grafo);

    return 0;
}
