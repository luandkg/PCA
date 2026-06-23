#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

#define INTERVALO_US 500  // intervalo de amostragem em microssegundos

long ler_vmrss_kb(pid_t pid) {
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "/proc/%d/status", pid);
    FILE *f = fopen(caminho, "r");
    if (!f) return -1;
    char linha[128];
    long kb = -1;
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "VmRSS: %ld kB", &kb) == 1) break;
    }
    fclose(f);
    return kb;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <programa> [args...]\n", argv[0]);
        return 1;
    }

    struct timespec t_inicio, t_fim;
    clock_gettime(CLOCK_MONOTONIC, &t_inicio);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // filho: executa o programa alvo
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    }

    // pai: monitora o filho
    long pico_kb = 0;
    long amostras = 0;

    while (1) {
        long rss = ler_vmrss_kb(pid);
        if (rss < 0) break;  // processo terminou
        if (rss > pico_kb) pico_kb = rss;
        amostras++;
        usleep(INTERVALO_US);

        // verifica se o filho terminou
        int status;
        pid_t resultado = waitpid(pid, &status, WNOHANG);
        if (resultado == pid) break;
    }

    clock_gettime(CLOCK_MONOTONIC, &t_fim);

    double tempo_ms = (t_fim.tv_sec - t_inicio.tv_sec) * 1000.0
                    + (t_fim.tv_nsec - t_inicio.tv_nsec) / 1e6;

    printf("\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("║           RELATORIO DO MONITOR           ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║ Programa    : %-27s║\n", argv[1]);
    printf("║ Tempo total : %-24.4f ms ║\n", tempo_ms);
    printf("║ Pico de RSS : %-24ld KB ║\n", pico_kb);
    printf("║ Amostras    : %-27ld║\n", amostras);
    printf("╚══════════════════════════════════════════╝\n");

    return 0;
}
