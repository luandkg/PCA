#!/usr/bin/env bash
# Roda cada combinação (algoritmo x dataset) pelo monitor e exibe tabela resumo.

set -uo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD="$PROJECT_DIR/build"
DADOS="$PROJECT_DIR/dados"
SAIDA="$PROJECT_DIR/benchmark_resultados.tsv"

ALGORITMOS=(
    "nw_recursivo_puro"
    "nw_recursivo_memoizado"
    "nw_dinamico"
)

# nw_recursivo_puro trava acima de t15; lista separada por algoritmo
TAMANHOS_PURO=(t5 t8 t10 t15)
TAMANHOS_OUTROS=(t5 t8 t10 t15 t20 t30 t100 t500 t1000)

echo "Algoritmo	Tamanho	Tempo_ms	Pico_RSS_KB" > "$SAIDA"

printf "%-30s %-10s %15s %15s\n" "Algoritmo" "Tamanho" "Tempo (ms)" "Pico RSS (KB)"
printf "%s\n" "--------------------------------------------------------------------------------"

rodar() {
    local algo="$1"
    local tam="$2"
    local seq1="$DADOS/sequencia_${tam}/seq1.dna"
    local seq2="$DADOS/sequencia_${tam}/seq2.dna"

    if [[ ! -f "$seq1" || ! -f "$seq2" ]]; then
        return
    fi

    local saida
    saida=$("$BUILD/monitor" "$BUILD/$algo" "$seq1" "$seq2" 2>/dev/null)

    local tempo pico
    tempo=$(echo "$saida" | grep "Tempo total" | grep -oE '[0-9]+\.[0-9]+' | head -1)
    pico=$(echo  "$saida" | grep "Pico de RSS" | grep -oE '[0-9]+' | head -1)

    printf "%-30s %-10s %15s %15s\n" "$algo" "$tam" "${tempo:-ERR}" "${pico:-ERR}"
    echo "${algo}	${tam}	${tempo:-ERR}	${pico:-ERR}" >> "$SAIDA"
}

for algo in "${ALGORITMOS[@]}"; do
    if [[ "$algo" == "nw_recursivo_puro" ]]; then
        tamanhos=("${TAMANHOS_PURO[@]}")
    else
        tamanhos=("${TAMANHOS_OUTROS[@]}")
    fi

    for tam in "${tamanhos[@]}"; do
        rodar "$algo" "$tam"
    done
done

printf "%s\n" "--------------------------------------------------------------------------------"
echo ""
echo "Resultados salvos em: $SAIDA"
