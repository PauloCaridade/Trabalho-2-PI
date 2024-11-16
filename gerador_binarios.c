#include "grafo.h"

// Função para gerar o arquivo binário de pontos a partir de um arquivo de texto
void gerar_binario_pontos(const char* arquivoTexto, const char* arquivoBin) {
    FILE* txt = fopen(arquivoTexto, "r");
    FILE* bin = fopen(arquivoBin, "wb");

    if (!txt || !bin) {
        printf("Erro ao abrir os arquivos de entrada/saída.\n");
        return;
    }

    int numPontos = 0;
    Ponto ponto;

    // Primeira leitura para contar o número de pontos
    while (fscanf(txt, " %c %f %f %s", &ponto.id, &ponto.x, &ponto.y, ponto.rua1,ponto.rua2) == 5) {
        numPontos++;
    }

    // Rewind para resetar o ponteiro do arquivo de texto
    rewind(txt);
    
    // Escreve o número de pontos no arquivo binário
    fwrite(&numPontos, sizeof(int), 1, bin);

    // Segunda leitura para gravar cada ponto no arquivo binário
    while (fscanf(txt, " %c %f %f %s", &ponto.id, &ponto.x, &ponto.y, ponto.rua1,ponto.rua2) == 5) {
        fwrite(&ponto, sizeof(Ponto), 1, bin);
    }

    fclose(txt);
    fclose(bin);
}

// Função para gerar o arquivo binário de vizinhos (arestas) a partir de um arquivo de texto
void gerar_binario_vizinhos(const char* arquivoTexto, const char* arquivoBin) {
    FILE* txt = fopen(arquivoTexto, "r");
    FILE* bin = fopen(arquivoBin, "wb");

    if (!txt || !bin) {
        printf("Erro ao abrir os arquivos de entrada/saída.\n");
        return;
    }

    int numArestas = 0;
    Aresta aresta;

    // Primeira leitura para contar o número de arestas
    while (fscanf(txt, " %c %c %s", &aresta.origem, &aresta.destino, aresta.nomeRua) == 3) {
        numArestas++;
    }

    // Rewind para resetar o ponteiro do arquivo de texto
    rewind(txt);

    // Escreve o número de arestas no arquivo binário
    fwrite(&numArestas, sizeof(int), 1, bin);

    // Segunda leitura para gravar cada aresta no arquivo binário
    while (fscanf(txt, " %c %c %s", &aresta.origem, &aresta.destino, aresta.nomeRua) == 3) {
        fwrite(&aresta, sizeof(Aresta), 1, bin);
    }

    fclose(txt);
    fclose(bin);
}

// Função principal para chamar as funções de geração de binários
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_pontos.txt> <arquivo_vizinhos.txt>\n", argv[0]);
        return 1;
    }

    gerar_binario_pontos(argv[1], "pontos.bin");
    gerar_binario_vizinhos(argv[2], "vizinhos.bin");

    printf("Arquivos binários gerados com sucesso.\n");
    return 0;
}
