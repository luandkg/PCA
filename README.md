# Relatório Técnico: Alinhamento Global de Sequências (Needleman-Wunsch)

## Análise Comparativa: Abordagem Recursiva Memoizada (Top-Down) vs. Iterativa (Bottom-Up)

Este documento apresenta a especificação teórica, a modelagem formal do problema e a análise de engenharia de software referentes à implementação do algoritmo de Needleman-Wunsch para alinhamento global de sequências biológicas.

---

## 1. Definição do Problema

### 1.1 Contextualização Biológica e Computacional

Na bioinformática, o alinhamento de sequências é o processo de comparar duas ou mais cadeias de caracteres (DNA, RNA ou proteínas) para identificar regiões de similaridade que possam indicar relações funcionais, estruturais ou evolutivas.

Computacionalmente, o **Alinhamento Global** busca alinhar as duas sequências do início ao fim, inserindo espaços vazios (denominados _Gaps_) onde houver inserções ou deleções históricas (indels), de modo a maximizar a correspondência de caracteres idênticos em posições equivalentes.

### 1.2 Formalização Matemática

Sejam duas sequências de caracteres:

- $A = (a_1, a_2, \dots, a_m)$ sobre um alfabeto $\Sigma$ (onde $\Sigma = \{A, T, C, G\}$).
- $B = (b_1, b_2, \dots, b_n)$ sobre o mesmo alfabeto $\Sigma$.

O problema consiste em encontrar uma matriz de correspondência estendida sobre um alfabeto modificado $\Sigma' = \Sigma \cup \{-\}$ (onde `_` representa o _Gap_), maximizando uma função de escore de similaridade global.

A métrica de qualidade do alinhamento é ditada por um modelo de pontuação linear simples:

- **Match ($S(a_i, b_j)$ se $a_i = b_j$):** $+1$
- **Mismatch ($S(a_i, b_j)$ se $a_i \neq b_j$):** $-1$
- **Gap ($g$):** $-1$

O escore final do alinhamento é a soma das pontuações atribuídas a cada par de caracteres alinhados. O objetivo primordial é encontrar o alinhamento cujo escore total seja máximo.

---

## 2. Técnica e Abordagem Algorítmica

O problema possui a propriedade da **Subestrutura Ótima** e de **Subproblemas Sobrepostos**. Isso significa que a solução ótima para alinhar os prefixos $A[1 \dots i]$ e $B[1 \dots j]$ depende diretamente das soluções ótimas de subproblemas menores. A relação de recorrência de Needleman-Wunsch estabelece uma matriz de escores $H$ de tamanho $(m+1) \times (n+1)$, governada pela equação:

$$H_{i,j} = \max \begin{cases} H_{i-1,j-1} + S(A_i, B_j) & \text{(Diagonal: Match ou Mismatch)} \\ H_{i-1,j} + g & \text{(Vertical: Deleção - Gap em B)} \\ H_{i,j-1} + g & \text{(Horizontal: Inserção - Gap em A)} \end{cases}$$

### 2.1 Técnica 1: Abordagem Recursiva com Memoização (_Top-Down_)

A recursão pura tenta resolver o problema partindo do objetivo final $H_{m,n}$ e ramificando o cálculo retroativamente para três subproblemas elementares: $(i-1, j-1)$, $(i-1, j)$, e $(i, j-1)$. Sem otimização, essa abordagem calcula as mesmas subposições repetidas vezes na árvore de chamadas, resultando em uma complexidade de tempo catastrófica de $\mathcal{O}(3^{m+n})$.

Para mitigar essa ineficiência sem abandonar o paradigma recursivo, aplica-se a técnica de **Memoização**:

1. Uma estrutura de dados em memória (matriz `M_visualizacao`) é instanciada e pré-preenchida com um sentinela (`UNKNOWN = -999999`).
2. No momento de cada chamada recursiva de `nw_recursivo(i, j)`, a função verifica se a posição consultada já possui um valor diferente de `UNKNOWN`.
3. Se o valor existir, ele é retornado em tempo constante $\mathcal{O}(1)$.
4. Se não existir, o cálculo recursivo tripartite é efetuado, o resultado máximo é armazenado na matriz correspondente e, finalmente, retornado.

### 2.2 Técnica 2: Abordagem Iterativa por Programação Dinâmica (_Bottom-Up_)

A abordagem iterativa inverte o fluxo do raciocínio. Em vez de partir do objetivo final, ela começa preenchendo as fundações estáticas da tabela (casos base de fronteira) e progride iterativamente em direção ao resultado.

1. Inicializa-se explicitamente a primeira linha e a primeira coluna multiplicando o índice da célula pelo custo acumulado de gaps ($i \times g$ e $j \times g$).
2. Utilizam-se dois laços de repetição (`for`) aninhados para varrer a matriz linha por linha, da esquerda para a direita.
3. Como o preenchimento segue uma ordem estrita e incremental, garante-se que, ao avaliar a célula $(i,j)$, os estados adjacentes necessários ($(i-1,j-1)$, $(i-1,j)$ e $(i,j-1)$) já foram devidamente consolidados na memória.

### 2.3 Mecânica do Traceback (Retrocesso)

Uma vez que a matriz $H$ está completamente preenchida por qualquer uma das técnicas, o escore ótimo absoluto está localizado na célula final $(m, n)$. No entanto, para obter a representação visual de quais bases se alinharam, executa-se o algoritmo de **Traceback**:

- Partindo de $(m, n)$, o algoritmo deduz retroativamente qual das três decisões (diagonal, cima ou esquerda) originou o valor da célula atual.
- A célula correspondente à decisão tomada é marcada na estrutura auxiliar booleana `Caminho_Traceback`.
- O ponteiro é deslocado para a célula de origem, repetindo o processo de forma cíclica até atingir a origem absoluta da matriz $(0,0)$.

---

### 3. Análise Comparativa de Desempenho

A tabela abaixo sumariza as métricas técnicas e limitações de engenharia entre as três estratégias de implementação:

| Métrica                    | Recursiva Pura      | Recursiva Memoizada   | Dinâmica (Iterativa)  |
| :------------------------- | :------------------ | :-------------------- | :-------------------- |
| **Complexidade de Tempo**  | $O(3^{m+n})$        | $O(m \times n)$       | $O(m \times n)$       |
| **Complexidade de Espaço** | $O(m + n)$          | $O(m \times n)$       | $O(m \times n)$       |
| **Uso da Pilha (Stack)**   | $O(m + n)$          | $O(m + n)$            | $O(1)$                |
| **Fluxo de Controle**      | Chamadas recursivas | Chamadas + Cache      | Laços `for` aninhados |
| **Limite de Sequência**    | $\approx 20$ bases  | $\approx 5.000$ bases | Limitado pela RAM     |
| **Risco de Falha**         | Stack Overflow      | Stack Overflow        | Out of Memory         |

### 3.1 Discussão Crítica da Análise

- **Igualdade Assintótica Teórica:** Introduzir a memoização converteu a complexidade de tempo da recursão de exponencial para quadrática $\mathcal{O}(m \times n)$. Isso ocorre porque cada uma das células da matriz é calculada exatamente uma vez e, nas consultas subsequentes, o custo torna-se constante.
- **Gargalo Físico da Pilha de Chamadas:** Apesar da igualdade no tempo de processamento, a arquitetura de execução diverge severamente. O programa recursivo armazena os contextos locais de variáveis, ponteiros de retorno e registradores na Pilha de Execução do Sistema Operacional para cada base processada. Para sequências biológicas reais (como genes de milhares de bases), o acúmulo de frames de função provoca uma falha crítica de _Stack Overflow_ (estouro de pilha).
- **Eficiência Mecânica:** A abordagem iterativa dispensa o gerenciamento complexo de frames de função no processador. Os laços `for` operam diretamente na memória Heap através de índices, explorando ao máximo a localidade de dados em cache e eliminando o _overhead_ de chamadas, tornando-se o paradigma padrão industrial para alinhamento biológico global.
