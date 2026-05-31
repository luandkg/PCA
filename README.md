# Needleman-Wunsch – Três Implementações em C

## 📌 Visão Geral

Este repositório contém três versões do algoritmo de **[Needleman-Wunsch]** para alinhamento global de sequências biológicas, implementadas em linguagem C:

1. **Recursiva Pura**
2. **Recursiva com Memoização**
3. **Programação Dinâmica Iterativa**

O objetivo é comparar desempenho, consumo de memória e aplicabilidade prática.

---

## ⚙️ Implementações

### 1. Recursiva Pura

- Resolve o problema chamando recursivamente subproblemas menores sem cache.
- **Complexidade de Tempo:** `O(3^(m+n))` (explosiva).
- **Limitações:** Só funciona para sequências muito pequenas (≈20 bases).
- **Risco:** Estouro de pilha (_Stack Overflow_).

### 2. Recursiva com Memoização

- Usa cache para armazenar resultados já calculados, evitando recomputações.
- **Complexidade de Tempo:** `O(m * n)`.
- **Complexidade de Espaço:** `O(m * n)`.
- **Limitações:** Ainda depende da pilha de chamadas, podendo falhar em sequências muito grandes.

### 3. Programação Dinâmica Iterativa

- Preenche a matriz de forma incremental com laços `for`.
- **Complexidade de Tempo:** `O(m * n)`.
- **Complexidade de Espaço:** `O(m * n)` (otimizável para `O(min(m,n))`).
- **Vantagens:** Escalável, eficiente em cache, padrão em bibliotecas industriais.

---

## 📊 Comparação

| Métrica            | Recursiva Pura | Recursiva Memoizada | Dinâmica Iterativa |
| :----------------- | :------------- | :------------------ | :----------------- |
| **Tempo**          | `O(3^(m+n))`   | `O(m * n)`          | `O(m * n)`         |
| **Espaço**         | `O(m+n)`       | `O(m * n)`          | `O(m * n)`         |
| **Uso da Pilha**   | Alto           | Alto                | Baixo              |
| **Limite Prático** | ≈20 bases      | ≈5.000 bases        | Limitado pela RAM  |
| **Risco de Falha** | Stack Overflow | Stack Overflow      | Out of Memory      |

---
