#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH     1
#define MISMATCH -1
#define GAP      -1
#define MAX_LEN  2048
#define UNKNOWN -999999

int **M_visualizacao;
int **Caminho_Traceback; 

// Retorna o maior valor entre três inteiros.
int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Algoritmo de Needleman-Wunsch recursivo com memoização (top-down O(m x n)).
// Calcula o score de alinhamento para prefixos de seqA e seqB de comprimento i e j.
int nw_recursivo_memoizado(const char *seqA, const char *seqB, int i, int j) {
    // Caso base: seqA está vazia, preencher a primeira linha com gaps.
    if (i == 0) {
        M_visualizacao[0][j] = j * GAP;
        return j * GAP;
    }
    // Caso base: seqB está vazia, preencher a primeira coluna com gaps.
    if (j == 0) {
        M_visualizacao[i][0] = i * GAP;
        return i * GAP;
    }

    // Se o valor já foi calculado antes, retorna diretamente para evitar recomputação.
    if (M_visualizacao[i][j] != UNKNOWN) {
        return M_visualizacao[i][j];
    }

    int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;

    // Calcula recursivamente os três possíveis caminhos de alinhamento.
    int diagonal = nw_recursivo_memoizado(seqA, seqB, i - 1, j - 1) + score_match;
    int delecao  = nw_recursivo_memoizado(seqA, seqB, i - 1, j) + GAP;
    int insercao = nw_recursivo_memoizado(seqA, seqB, i, j - 1) + GAP;

    int resultado = max3(diagonal, delecao, insercao);
    M_visualizacao[i][j] = resultado;
    
    return resultado;
}

// Reconstrói o caminho ótimo a partir da célula final (m, n) usando a matriz de pontuações.
// Marca em Caminho_Traceback as células que fazem parte do alinhamento ideal.
void calcular_traceback(const char *seqA, const char *seqB) {
    int i = strlen(seqA);
    int j = strlen(seqB);

    while (i > 0 || j > 0) {
        Caminho_Traceback[i][j] = 1; 

        if (i > 0 && j > 0) {
            int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;
            if (M_visualizacao[i][j] == M_visualizacao[i - 1][j - 1] + score_match) {
                i--; j--;
                continue;
            }
        }
        if (i > 0 && M_visualizacao[i][j] == M_visualizacao[i - 1][j] + GAP) {
            i--;
        } 
        else if (j > 0 && M_visualizacao[i][j] == M_visualizacao[i][j - 1] + GAP) {
            j--;
        }
    }
    Caminho_Traceback[0][0] = 1; 
}

// Exibe a matriz de pontuações para o alinhamento com destaque das células do traceback.
// O asterisco indica as células que fazem parte do caminho ótimo.
void imprimir_matriz(const char *seqA, const char *seqB) {
    int m = strlen(seqA);
    int n = strlen(seqB);

    printf("\n┌───────────────────────────────────────────┐");
    printf("\n│  TABELA DE ALINHAMENTO (MEMOIZADO com *)  │");
    printf("\n└───────────────────────────────────────────┘\n\n");
    
    printf("┌──────┬──────┬");
    for (int j = 0; j < n; j++) {
        if (j == n - 1) printf("──────┐\n");
        else printf("──────┬");
    }
    
    printf("│      │   -  │");
    for (int j = 0; j < n; j++) {
        printf("  %c   │", seqB[j]);
    }
    printf("\n");

    printf("├──────┼──────┼");
    for (int j = 0; j < n; j++) {
        if (j == n - 1) printf("──────┤\n");
        else printf("──────┼");
    }

    for (int i = 0; i <= m; i++) {
        if (i == 0) printf("│   -  │");
        else printf("│   %c  │", seqA[i - 1]);

        for (int j = 0; j <= n; j++) {
            if (Caminho_Traceback[i][j]) {
                printf(" %3d* │", M_visualizacao[i][j]);
            } else {
                printf(" %4d │", M_visualizacao[i][j]);
            }
        }
        printf("\n");

        if (i == m) {
            printf("└──────┴──────┴");
            for (int j = 0; j < n; j++) {
                if (j == n - 1) printf("──────┘\n");
                else printf("──────┴");
            }
        } else {
            printf("├──────┼──────┼");
            for (int j = 0; j < n; j++) {
                if (j == n - 1) printf("──────┤\n");
                else printf("──────┼");
            }
        }
    }
    printf("\n");
}

// Lê uma sequência de um arquivo e remove o caractere de nova linha ao final.
int ler_sequencia(const char *filename, char *buffer) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", filename);
        return 0;
    }
    if (fgets(buffer, MAX_LEN, file) == NULL) {
        fprintf(stderr, "Arquivo vazio ou ilegível: %s\n", filename);
        fclose(file);
        return 0;
    }
    fclose(file);
    buffer[strcspn(buffer, "\r\n")] = 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_sequencia_A> <arquivo_sequencia_B>\n", argv[0]);
        return 1;
    }

    char seqA[MAX_LEN];
    char seqB[MAX_LEN];

    // Lê as duas sequências de entrada a partir dos arquivos informados.

    if (!ler_sequencia(argv[1], seqA) || !ler_sequencia(argv[2], seqB)) {
        return 1;
    }

    int m = strlen(seqA);
    int n = strlen(seqB);

    // Inicializa as dimensões do problema.

    // Aloca memória para a matriz de pontuações e para a matriz de traceback.
    M_visualizacao = (int **)malloc((m + 1) * sizeof(int *));
    Caminho_Traceback = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        M_visualizacao[i] = (int *)malloc((n + 1) * sizeof(int));
        Caminho_Traceback[i] = (int *)calloc((n + 1), sizeof(int));
        for (int j = 0; j <= n; j++) {
            M_visualizacao[i][j] = UNKNOWN; 
        }
    }

    int score = nw_recursivo_memoizado(seqA, seqB, m, n);
    
    // Ajusta as bordas da matriz com valores de gap, garantindo que os casos base fiquem corretos.
    for (int i = 0; i <= m; i++) M_visualizacao[i][0] = i * GAP;
    for (int j = 0; j <= n; j++) M_visualizacao[0][j] = j * GAP;

    // Reconstrói o caminho ótimo e exibe a matriz de alinhamento completa.
    calcular_traceback(seqA, seqB);
    imprimir_matriz(seqA, seqB);
    printf("[NW RECURSIVO MEMOIZADO] Score: %d\n", score);

    // Libera toda a memória alocada dinamicamente.
    for (int i = 0; i <= m; i++) {
        free(M_visualizacao[i]);
        free(Caminho_Traceback[i]);
    }
    free(M_visualizacao);
    free(Caminho_Traceback);

    return 0;
}