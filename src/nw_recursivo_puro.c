#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH     1
#define MISMATCH -1
#define GAP      -1
#define MAX_LEN  2048

int **M_visualizacao;
int **Caminho_Traceback; // Matriz auxiliar para marcar o caminho do Traceback

int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Algoritmo Recursivo Puro
int nw_recursivo_puro(const char *seqA, const char *seqB, int i, int j) {
    if (i == 0) {
        M_visualizacao[0][j] = j * GAP;
        return j * GAP;
    }
    if (j == 0) {
        M_visualizacao[i][0] = i * GAP;
        return i * GAP;
    }

    int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;

    int diagonal = nw_recursivo_puro(seqA, seqB, i - 1, j - 1) + score_match;
    int delecao  = nw_recursivo_puro(seqA, seqB, i - 1, j) + GAP;
    int insercao = nw_recursivo_puro(seqA, seqB, i, j - 1) + GAP;

    int resultado = max3(diagonal, delecao, insercao);
    M_visualizacao[i][j] = resultado;
    
    return resultado;
}

// Reconstrói o caminho ótimo após o preenchimento da matriz
void calcular_traceback(const char *seqA, const char *seqB) {
    int i = strlen(seqA);
    int j = strlen(seqB);

    while (i > 0 || j > 0) {
        Caminho_Traceback[i][j] = 1; // Marca a célula atual

        if (i > 0 && j > 0) {
            int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;
            if (M_visualizacao[i][j] == M_visualizacao[i - 1][j - 1] + score_match) {
                i--; j--; continue;
            }
        }
        if (i > 0 && M_visualizacao[i][j] == M_visualizacao[i - 1][j] + GAP) {
            i--;
        } else if (j > 0 && M_visualizacao[i][j] == M_visualizacao[i][j - 1] + GAP) {
            j--;
        }
    }
    Caminho_Traceback[0][0] = 1;
}

void imprimir_matriz(const char *seqA, const char *seqB) {
    int m = strlen(seqA);
    int n = strlen(seqB);

    printf("\n┌───────────────────────────────────────────┐");
    printf("\n│   TABELA DE ALINHAMENTO (RECURSIVO PURO*) │");
    printf("\n└───────────────────────────────────────────┘\n\n");
    
    // Cabeçalho da Tabela
    printf("┌──────┬──────┬");
    for (int j = 0; j < n; j++) printf("──────┬");
    printf("\n│      │   -  │");
    for (int j = 0; j < n; j++) printf("  %c   │", seqB[j]);
    printf("\n├──────┼──────┼");
    for (int j = 0; j < n; j++) printf("──────┼");
    printf("\n");

    // Conteúdo da Tabela
    for (int i = 0; i <= m; i++) {
        if (i == 0) printf("│   -  │");
        else printf("│   %c  │", seqA[i - 1]);

        for (int j = 0; j <= n; j++) {
            if (Caminho_Traceback[i][j]) printf(" %3d* │", M_visualizacao[i][j]);
            else printf(" %4d │", M_visualizacao[i][j]);
        }
        printf("\n├──────┼──────┼");
        for (int j = 0; j < n; j++) printf("──────┼");
        printf("\n");
    }
}

int ler_sequencia(const char *filename, char *buffer) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", filename);
        return 0;
    }
    if (fgets(buffer, MAX_LEN, file) == NULL) {
        fclose(file);
        return 0;
    }
    fclose(file);
    buffer[strcspn(buffer, "\r\n")] = 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;

    char query[MAX_LEN], db[MAX_LEN];
    ler_sequencia(argv[1], query);
    ler_sequencia(argv[2], db);
    int m = strlen(query), n = strlen(db);

    if (m > 15 || n > 15) {
        printf("Limite excedido: Recursão pura limitada a 15 caracteres.\n");
        return 1;
    }

    M_visualizacao = (int **)malloc((m + 1) * sizeof(int *));
    Caminho_Traceback = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        M_visualizacao[i] = (int *)calloc((n + 1), sizeof(int));
        Caminho_Traceback[i] = (int *)calloc((n + 1), sizeof(int));
    }

    int score = nw_recursivo_puro(query, db, m, n);
    calcular_traceback(query, db);
    imprimir_matriz(query, db);
    
    printf("\n[NW RECURSIVO PURO] Score: %d\n", score);

    for (int i = 0; i <= m; i++) {
        free(M_visualizacao[i]);
        free(Caminho_Traceback[i]);
    }
    free(M_visualizacao);
    free(Caminho_Traceback);

    return 0;
}