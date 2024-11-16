#include "grafo.h"


void binario_pontos(const char* arquivoTexto, const char* arquivoBin) 
{
    FILE* txt = fopen(arquivoTexto, "r");
    FILE* bin = fopen(arquivoBin, "wb");

    if (!txt || !bin) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    int numPontos = 0;
    Ponto ponto;


    while (fscanf(txt, " %c %f %f %[^\n] %[^\n]", &ponto.id, &ponto.x, &ponto.y, ponto.rua1, ponto.rua2) == 5) {
        numPontos++;
    }

    rewind(txt);
    
    fwrite(&numPontos, sizeof(int), 1, bin);

    while (fscanf(txt, " %c %f %f %[^\n] %[^\n]", &ponto.id, &ponto.x, &ponto.y, ponto.rua1, ponto.rua2) == 5) {
        fwrite(&ponto, sizeof(Ponto), 1, bin);
    }

    fclose(txt);
    fclose(bin);
}


void binario_vizinhos(const char* arquivoTexto, const char* arquivoBin) 
{
    FILE* txt = fopen(arquivoTexto, "r");
    FILE* bin = fopen(arquivoBin, "wb");

    if (!txt || !bin) {
        printf("Erro ao abrir os arquivos de entrada/saída.\n");
        return;
    }

    int numArestas = 0;
    Aresta aresta;

    
    while (fscanf(txt, " %c %c %[^\n]", &aresta.origem, &aresta.destino, aresta.nomeRua) == 3) {
        numArestas++;
    }

    
    rewind(txt);

   
    fwrite(&numArestas, sizeof(int), 1, bin);

  
    while (fscanf(txt, " %c %c %[^\n]", &aresta.origem, &aresta.destino, aresta.nomeRua) == 3) {
        fwrite(&aresta, sizeof(Aresta), 1, bin);
    }

    fclose(txt);
    fclose(bin);
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_pontos.txt> <arquivo_vizinhos.txt>\n", argv[0]);
        return 1;
    }

    binario_pontos(argv[1], "pontos.bin");
    binario_vizinhos(argv[2], "vizinhos.bin");

    printf("Os arquivos binarios foram gerados sem erros\n");
    return 0;
}
