# 🚀 Comandos de Execução por Cenário

## 1. Cenário Tamanho 8 (`t8`)

Ideal para validação visual e explicação rápida, pois a tabela inteira cabe na tela do terminal sem necessidade de rolagem.

```bash
# Teste com Recursão Pura (Sem otimização)
./build/nw_recursivo_puro 'dados/sequencia_t8/seq1.dna' 'dados/sequencia_t8/seq2.dna'

# Teste com Recursão Memoizada (Otimizada, destaca o Traceback com *)
./build/nw_recursivo_memoizado 'dados/sequencia_t8/seq1.dna' 'dados/sequencia_t8/seq2.dna'

# Teste com Programação Dinâmica Iterativa (Matriz completa com *)
./build/nw_dinamico 'dados/sequencia_t8/seq1.dna' 'dados/sequencia_t8/seq2.dna'

```

## 2. Cenário Tamanho 15 (`t15`)

O limite estável para a árvore de recursão pura. Excelente para demonstrar o impacto do crescimento exponencial na CPU.

```bash
# O executável puro calculará a árvore completa (pode apresentar um leve delay de processamento)
./build/nw_recursivo_puro 'dados/sequencia_t15/seq1.dna' 'dados/sequencia_t15/seq2.dna'

# O memoizado resolve o mesmo problema instantaneamente devido ao reaproveitamento de estados
./build/nw_recursivo_memoizado 'dados/sequencia_t15/seq1.dna' 'dados/sequencia_t15/seq2.dna'

# O dinâmico resolve de forma linear e iterativa através de loops for
./build/nw_dinamico 'dados/sequencia_t15/seq1.dna' 'dados/sequencia_t15/seq2.dna'

```

## 3. Cenário Tamanho 30 (`t30`)

A prova de conceito definitiva. Demonstra o colapso do algoritmo de força bruta e o sucesso das técnicas de Programação Dinâmica e Memoização.

```bash
# ❌ ESTE COMANDO RETORNARÁ UM ERRO DE BLOQUEIO DE SEGURANÇA (Evita o travamento do processador)
./build/nw_recursivo_puro 'dados/sequencia_t30/seq1.dna' 'dados/sequencia_t30/seq2.dna'

# 🟢 EXECUÇÃO IMEDIATA: A árvore de recursão é podada e o resultado sai na hora
./build/nw_recursivo_memoizado 'dados/sequencia_t30/seq1.dna' 'dados/sequencia_t30/seq2.dna'

# 🟢 EXECUÇÃO IMEDIATA: Abordagem matricial clássica, estável e extremamente veloz
./build/nw_dinamico 'dados/sequencia_t30/seq1.dna' 'dados/sequencia_t30/seq2.dna'

```
