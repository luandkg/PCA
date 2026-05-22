#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH     1
#define MISMATCH -1
#define GAP      -1
#define MAX_LEN  2048

int **M_visualizacao;
int **Caminho_Traceback; // Matriz auxiliar para marcar o caminho do Traceback

// Retorna o maior entre três valores inteiros.
int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Algoritmo Recursivo Puro de Needleman-Wunsch.
// Calcula o score de alinhamento para prefixos de seqA e seqB de tamanho i e j.
// A matriz M_visualizacao é preenchida com valores de pontuação para visualização.
int nw_recursivo_puro(const char *seqA, const char *seqB, int i, int j) {
    // Caso base: seqA esvaziou, preenche coluna j com penalidades de gaps.
    if (i == 0) {
        M_visualizacao[0][j] = j * GAP;
        return j * GAP;
    }
    // Caso base: seqB esvaziou, preenche linha i com penalidades de gaps.
    if (j == 0) {
        M_visualizacao[i][0] = i * GAP;
        return i * GAP;
    }

    // Determina se os caracteres atuais formam match ou mismatch.
    int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;

    // Explora as três possibilidades de transição recursiva:
    // 1) diagonal (alinhamento dos dois caracteres)
    // 2) deleção (gap em seqB)
    // 3) inserção (gap em seqA)
    int diagonal = nw_recursivo_puro(seqA, seqB, i - 1, j - 1) + score_match;
    int delecao  = nw_recursivo_puro(seqA, seqB, i - 1, j) + GAP;
    int insercao = nw_recursivo_puro(seqA, seqB, i, j - 1) + GAP;

    int resultado = max3(diagonal, delecao, insercao);
    M_visualizacao[i][j] = resultado;
    
    return resultado;
}

// Reconstrói o caminho ótimo usando traceback a partir da célula final (m, n).
// Marca as células que participam do alinhamento ideal em Caminho_Traceback.
void calcular_traceback(const char *seqA, const char *seqB) {
    int i = strlen(seqA);
    int j = strlen(seqB);

    while (i > 0 || j > 0) {
        Caminho_Traceback[i][j] = 1; // Marca a célula atual como parte do caminho.

        if (i > 0 && j > 0) {
            int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;
            // Move pela diagonal quando o alinhamento deriva de uma correspondência ou mismatch.
            if (M_visualizacao[i][j] == M_visualizacao[i - 1][j - 1] + score_match) {
                i--; j--; continue;
            }
        }
        // Se não for diagonal, verifica se o caminho é proveniente de uma deleção.
        if (i > 0 && M_visualizacao[i][j] == M_visualizacao[i - 1][j] + GAP) {
            i--;
        } else if (j > 0 && M_visualizacao[i][j] == M_visualizacao[i][j - 1] + GAP) {
            // Caso contrário, assume que veio de uma inserção.
            j--;
        }
    }
    Caminho_Traceback[0][0] = 1; // Marca a origem do traceback.
}

// Exibe a matriz de alinhamento na saída padrão.
// Células marcadas no traceback são destacadas com um asterisco.
void imprimir_matriz(const char *seqA, const char *seqB) {
    int m = strlen(seqA);
    int n = strlen(seqB);

    printf("\n┌───────────────────────────────────────────┐");
    printf("\n│   TABELA DE ALINHAMENTO (RECURSIVO PURO*) │");
    printf("\n└───────────────────────────────────────────┘\n\n");
    
    // Cabeçalho da tabela com seqB alinhada ao longo das colunas.
    printf("┌──────┬──────┬");
    for (int j = 0; j < n; j++) printf("──────┬");
    printf("\n│      │   -  │");
    for (int j = 0; j < n; j++) printf("  %c   │", seqB[j]);
    printf("\n├──────┼──────┼");
    for (int j = 0; j < n; j++) printf("──────┼");
    printf("\n");

    // Linhas da tabela correspondentes a seqA e aos valores da matriz.
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

// Lê uma sequência a partir de um arquivo e remove o caractere de nova linha.
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

    // Lê as duas sequências a partir dos arquivos informados nos argumentos.
    char seqA[MAX_LEN], seqB[MAX_LEN];
    ler_sequencia(argv[1], seqA);
    ler_sequencia(argv[2], seqB);
    int m = strlen(seqA), n = strlen(seqB);

    // Evita usar recursão pura para sequências muito longas.
    if (m > 15 || n > 15) {
        printf("Limite excedido: Recursão pura limitada a 15 caracteres.\n");
        return 1;
    }

    // Aloca memória dinâmica para as matrizes de pontuação e traceback.
    M_visualizacao = (int **)malloc((m + 1) * sizeof(int *));
    Caminho_Traceback = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        M_visualizacao[i] = (int *)calloc((n + 1), sizeof(int));
        Caminho_Traceback[i] = (int *)calloc((n + 1), sizeof(int));
    }

    // Executa o alinhamento recursivo puro e calcula o traceback.
    int score = nw_recursivo_puro(seqA, seqB, m, n);
    calcular_traceback(seqA, seqB);
    imprimir_matriz(seqA, seqB);
    
    printf("\n[NW RECURSIVO PURO] Score: %d\n", score);

    // Libera a memória alocada para evitar vazamentos.
    for (int i = 0; i <= m; i++) {
        free(M_visualizacao[i]);
        free(Caminho_Traceback[i]);
    }
    free(M_visualizacao);
    free(Caminho_Traceback);

    return 0;
}