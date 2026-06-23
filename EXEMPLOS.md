# Comandos de Execução por Cenário

## 1. Cenário Tamanho 8 (`t8`)

Ideal para validação visual e explicação rápida, pois a tabela inteira cabe na tela do terminal sem necessidade de rolagem.

```bash
# Teste com Recursão Pura (Sem otimização, destaca o Traceback com *)
./build/nw_recursivo_puro dados/sequencia_t8/seq1.dna dados/sequencia_t8/seq2.dna

# Teste com Recursão Memoizada (Otimizada, destaca o Traceback com *)
./build/nw_recursivo_memoizado dados/sequencia_t8/seq1.dna dados/sequencia_t8/seq2.dna

# Teste com Programação Dinâmica Iterativa (Matriz completa com *)
./build/nw_dinamico dados/sequencia_t8/seq1.dna dados/sequencia_t8/seq2.dna
```

---

## 2. Cenário Tamanho 15 (`t15`)

O limite estável para a árvore de recursão pura. Excelente para demonstrar o impacto do crescimento exponencial na CPU.

```bash
# O executável puro calculará a árvore completa (pode apresentar um leve delay de processamento)
./build/nw_recursivo_puro dados/sequencia_t15/seq1.dna dados/sequencia_t15/seq2.dna

# O memoizado resolve o mesmo problema instantaneamente devido ao reaproveitamento de estados
./build/nw_recursivo_memoizado dados/sequencia_t15/seq1.dna dados/sequencia_t15/seq2.dna

# O dinâmico resolve de forma linear e iterativa através de loops for
./build/nw_dinamico dados/sequencia_t15/seq1.dna dados/sequencia_t15/seq2.dna
```

---

## 3. Cenário Tamanho 30 (`t30`)

A prova de conceito definitiva. Demonstra o colapso do algoritmo de força bruta e o sucesso das técnicas de Programação Dinâmica e Memoização.

```bash
# [BLOQUEADO] Este comando retornará um erro de bloqueio de segurança
./build/nw_recursivo_puro dados/sequencia_t30/seq1.dna dados/sequencia_t30/seq2.dna

# [OK] Execução imediata: a arvore de recursão é podada e o resultado sai na hora
./build/nw_recursivo_memoizado dados/sequencia_t30/seq1.dna dados/sequencia_t30/seq2.dna

# [OK] Execução imediata: abordagem matricial clássica, estável e extremamente veloz
./build/nw_dinamico dados/sequencia_t30/seq1.dna dados/sequencia_t30/seq2.dna
```

---

## 4. Cenário Tamanho 5 (`t5`)

Útil para demonstração ultra-rápida ou quando o tempo de tela é limitado. A matriz 6x6 é trivial e serve para validar a corretude da inicialização de bordas.

```bash
./build/nw_recursivo_puro dados/sequencia_t5/seq1.dna dados/sequencia_t5/seq2.dna
./build/nw_recursivo_memoizado dados/sequencia_t5/seq1.dna dados/sequencia_t5/seq2.dna
./build/nw_dinamico dados/sequencia_t5/seq1.dna dados/sequencia_t5/seq2.dna
```

**Tempo medido (monitor):** todos os algoritmos < 1 ms | **Pico de RAM:** Puro 1740 KB · Memo 940 KB · Bottom-Up 420 KB

---

## 5. Cenário Tamanho 10 (`t10`)

Ponto de inflexão para a Recursão Pura: já apresenta ~2,4 s de execução, evidenciando o início do crescimento exponencial mesmo para sequências curtas.

```bash
# [LENTO] ~2,4 s de execução para apenas 10 bases
./build/nw_recursivo_puro dados/sequencia_t10/seq1.dna dados/sequencia_t10/seq2.dna

# [OK] Instantâneo
./build/nw_recursivo_memoizado dados/sequencia_t10/seq1.dna dados/sequencia_t10/seq2.dna

# [OK] Instantâneo
./build/nw_dinamico dados/sequencia_t10/seq1.dna dados/sequencia_t10/seq2.dna
```

**Tempo medido (monitor):** Puro ~2,4 s · Memo < 1 ms · Bottom-Up < 1 ms | **Pico de RAM:** Puro 1744 KB · Memo 1356 KB · Bottom-Up 736 KB

---

## 6. Cenário Tamanho 20 (`t20`)

A Recursão Pura estimaria ~45 min de execução (extrapolado) e é bloqueada por limite de segurança. Memoização e Bottom-Up seguem instantâneos.

```bash
# [BLOQUEADO] Estimativa: ~45 min — execução bloqueada por limite de segurança
./build/nw_recursivo_puro dados/sequencia_t20/seq1.dna dados/sequencia_t20/seq2.dna

# [OK] Instantâneo
./build/nw_recursivo_memoizado dados/sequencia_t20/seq1.dna dados/sequencia_t20/seq2.dna

# [OK] Instantâneo
./build/nw_dinamico dados/sequencia_t20/seq1.dna dados/sequencia_t20/seq2.dna
```

**Tempo medido (monitor):** Puro inviável · Memo < 1 ms · Bottom-Up < 1 ms | **Pico de RAM:** n/a · Memo 1740 KB · Bottom-Up 412 KB

---

## 7. Cenário Tamanho 100 (`t100`)

Primeiro tamanho onde o Bottom-Up apresenta tempo mensurável (~9 ms). Memoização ainda abaixo de 1 ms. Recursão pura completamente inviável.

```bash
# [INVIAVEL]
./build/nw_recursivo_puro dados/sequencia_t100/seq1.dna dados/sequencia_t100/seq2.dna

# [OK] Instantâneo (< 1 ms)
./build/nw_recursivo_memoizado dados/sequencia_t100/seq1.dna dados/sequencia_t100/seq2.dna

# [OK] Rapido (~9 ms)
./build/nw_dinamico dados/sequencia_t100/seq1.dna dados/sequencia_t100/seq2.dna
```

**Tempo medido (monitor):** Puro inviável · Memo < 1 ms · Bottom-Up ~9 ms | **Pico de RAM:** n/a · Memo 1844 KB · Bottom-Up 1840 KB

---

## 8. Cenário Tamanho 500 (`t500`)

A diferença entre Memoização e Bottom-Up fica evidente. O comportamento lazy do Top-Down (~5 ms) supera o Bottom-Up (~119 ms) por preencher apenas os estados necessários ao traceback.

```bash
# [INVIAVEL]
./build/nw_recursivo_puro dados/sequencia_t500/seq1.dna dados/sequencia_t500/seq2.dna

# [OK] Rapido (~5 ms) — lazy evaluation evita preencher a matriz toda
./build/nw_recursivo_memoizado dados/sequencia_t500/seq1.dna dados/sequencia_t500/seq2.dna

# [OK] Mais lento (~119 ms) — preenche a matriz 501x501 completa
./build/nw_dinamico dados/sequencia_t500/seq1.dna dados/sequencia_t500/seq2.dna
```

**Tempo medido (monitor):** Puro inviável · Memo ~5 ms · Bottom-Up ~119 ms | **Pico de RAM:** n/a · Memo 3492 KB · Bottom-Up 3460 KB

---

## 9. Cenário Tamanho 1000 (`t1000`)

Maior conjunto de dados disponível. Demonstra o crescimento O(m x n) de RAM em ambas as abordagens polinomiais (~6,5 MB). O Bottom-Up (~341 ms) paga o custo de preencher a matriz 1001x1001 inteira, enquanto o Top-Down (~17 ms) permanece muito mais eficiente na prática.

```bash
# [INVIAVEL]
./build/nw_recursivo_puro dados/sequencia_t1000/seq1.dna dados/sequencia_t1000/seq2.dna

# [OK] Eficiente (~17 ms)
./build/nw_recursivo_memoizado dados/sequencia_t1000/seq1.dna dados/sequencia_t1000/seq2.dna

# [OK] Correto porem mais lento (~341 ms)
./build/nw_dinamico dados/sequencia_t1000/seq1.dna dados/sequencia_t1000/seq2.dna
```

**Tempo medido (monitor):** Puro inviável · Memo ~17 ms · Bottom-Up ~341 ms | **Pico de RAM:** n/a · Memo 6652 KB · Bottom-Up 6592 KB

---

## Monitoramento de Tempo e Memoria

Para medir tempo de execução e pico de RAM de qualquer combinação, use o `monitor`:

```bash
# Sintaxe: ./build/monitor <executavel> <seq1> <seq2>
./build/monitor ./build/nw_dinamico dados/sequencia_t1000/seq1.dna dados/sequencia_t1000/seq2.dna
```

Para rodar todos os cenários e gerar a tabela comparativa completa:

```bash
bash ./benchmark.sh
# Resultado salvo em: benchmark_resultados.tsv
```

---

## Tabela Comparativa: Número Estimado de Operações

| Tamanho | nw_recursivo_puro      | nw_recursivo_memoizado | nw_dinamico |
| ------- | ---------------------- | ---------------------- | ----------- |
| T5      | ~243                   | 36                     | 36          |
| T8      | ~43.046.721            | 81                     | 81          |
| T10     | ~3.486.784.401         | 121                    | 121         |
| T15     | ~205.891.132.094.649   | 256                    | 256         |
| T20     | ~3,49 x 10^19          | 441                    | 441         |
| T30     | ~1,23 x 10^28          | 961                    | 961         |
| T100    | inviavel               | 10.201                 | 10.201      |
| T500    | inviavel               | 251.001                | 251.001     |
| T1000   | inviavel               | 1.002.001              | 1.002.001   |

---

## Tabela Comparativa: Resultados Medidos pelo monitor

| Tamanho | Puro (tempo) | Memo (tempo) | Bottom-Up (tempo) | Puro (RAM KB) | Memo (RAM KB) | Bottom-Up (RAM KB) |
| ------- | ------------ | ------------ | ----------------- | ------------- | ------------- | ------------------ |
| 5       | < 1 ms       | < 1 ms       | < 1 ms (*)        | 1740          | 940           | 420                |
| 10      | ~2,4 s       | < 1 ms       | < 1 ms (*)        | 1744          | 1356          | 736                |
| 15      | ~6,9 min     | < 1 ms       | < 1 ms (*)        | 1744          | 1212          | 1748               |
| 20      | ~45 min (**)  | < 1 ms       | < 1 ms (*)        | n/a           | 1740          | 412                |
| 100     | inviavel     | < 1 ms       | ~9 ms             | n/a           | 1844          | 1840               |
| 500     | inviavel     | ~5 ms        | ~119 ms           | n/a           | 3492          | 3460               |
| 1000    | inviavel     | ~17 ms       | ~341 ms           | n/a           | 6652          | 6592               |

(*) Overhead do monitor domina; algoritmo < 1 ms.
(**) Estimativa extrapolada — execução bloqueada por limite de segurança.

---

"Os resultados são idênticos porque ambos os algoritmos calculam exatamente a mesma função de recorrência matemática. A diferença entre eles é puramente estrutural: um utiliza a pilha de chamadas e memoização para preencher os estados sob demanda (Top-Down), enquanto o outro preenche a matriz de forma sequencial e iterativa (Bottom-Up). A identidade dos resultados é a validação de que a lógica de ambos os métodos está correta."
