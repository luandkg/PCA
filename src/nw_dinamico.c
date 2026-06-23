#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

long vmrss_kb() {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return -1;
    char linha[128];
    long kb = -1;
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "VmRSS: %ld kB", &kb) == 1) break;
    }
    fclose(f);
    return kb;
}

#define MATCH     1
#define MISMATCH -1
#define GAP      -1
#define MAX_LEN  2048

int max3(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

// Implementação de Needleman-Wunsch por programação dinâmica.
// Preenche a matriz de pontuações H e reconstrói o caminho ótimo.
void nw_programacao_dinamica(const char *seqA, const char *seqB) {
    int m = strlen(seqA);
    int n = strlen(seqB);

    // Aloca as matrizes de pontuação e de traceback com dimensões (m+1) x (n+1).
    int **H = (int **)malloc((m + 1) * sizeof(int *));
    int **Caminho_Traceback = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        H[i] = (int *)calloc((n + 1), sizeof(int));
        Caminho_Traceback[i] = (int *)calloc((n + 1), sizeof(int));
    }

    // Inicializa as bordas da matriz com penalidades de gap.
    for (int i = 0; i <= m; i++) H[i][0] = i * GAP;
    for (int j = 0; j <= n; j++) H[0][j] = j * GAP;

    // Calcula os valores da matriz linha a linha, usando os subproblemas resolvidos previamente.
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int score_match = (seqA[i - 1] == seqB[j - 1]) ? MATCH : MISMATCH;
            
            H[i][j] = max3(
                H[i - 1][j - 1] + score_match,
                H[i - 1][j] + GAP,            
                H[i][j - 1] + GAP             
            );
        }
    }

    // Reconstrói o caminho do alinhamento ótimo a partir da célula final.
    int i_tr = m, j_tr = n;
    while (i_tr > 0 || j_tr > 0) {
        Caminho_Traceback[i_tr][j_tr] = 1;

        if (i_tr > 0 && j_tr > 0) {
            int score_match = (seqA[i_tr - 1] == seqB[j_tr - 1]) ? MATCH : MISMATCH;
            if (H[i_tr][j_tr] == H[i_tr - 1][j_tr - 1] + score_match) {
                i_tr--; j_tr--;
                continue;
            }
        }
        if (i_tr > 0 && H[i_tr][j_tr] == H[i_tr - 1][j_tr] + GAP) {
            i_tr--;
        } else if (j_tr > 0 && H[i_tr][j_tr] == H[i_tr][j_tr - 1] + GAP) {
            j_tr--;
        }
    }
    Caminho_Traceback[0][0] = 1;

    printf("\n┌───────────────────────────────────────────┐");
    printf("\n│    TABELA DE ALINHAMENTO (PROG. DINÂMICA) │");
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
                printf(" %3d* │", H[i][j]);
            } else {
                printf(" %4d │", H[i][j]);
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

    printf("[NW PROG. DINAMICA] Score: %d\n", H[m][n]);

    // Libera a memória usada pelas matrizes H e Caminho_Traceback.
    for (int i = 0; i <= m; i++) {
        free(H[i]);
        free(Caminho_Traceback[i]);
    }
    free(H);
    free(Caminho_Traceback);
}

// Lê uma sequência de um arquivo e remove o caractere de nova linha final.
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

    // Executa o alinhamento por programação dinâmica.
    long mem_antes = vmrss_kb();
    clock_t inicio = clock();
    nw_programacao_dinamica(seqA, seqB);
    clock_t fim = clock();
    long mem_depois = vmrss_kb();

    double tempo_ms = (double)(fim - inicio) / CLOCKS_PER_SEC * 1000.0;
    printf("[NW PROG. DINAMICA] Tempo de execucao: %.4f ms\n", tempo_ms);
    printf("[NW PROG. DINAMICA] Memoria (RSS antes: %ld KB | depois: %ld KB | delta: %ld KB)\n",
           mem_antes, mem_depois, mem_depois - mem_antes);

    return 0;
}