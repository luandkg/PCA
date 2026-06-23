# --- Variáveis de Configuração ---
CC = gcc
CFLAGS = -Wall -Wextra -O3
BUILD_DIR = build
SRC_DIR = src

# Nomes dos executáveis finais dentro da pasta build
TARGET_PURO    = $(BUILD_DIR)/nw_recursivo_puro
TARGET_MEMO    = $(BUILD_DIR)/nw_recursivo_memoizado
TARGET_DYN     = $(BUILD_DIR)/nw_dinamico
TARGET_MONITOR = $(BUILD_DIR)/monitor

# --- Regras Principais ---

# Regra padrão: Compila os três códigos
all: create_build_dir $(TARGET_PURO) $(TARGET_MEMO) $(TARGET_DYN) $(TARGET_MONITOR)
	@echo "================================================================="
	@echo " Sucesso! Todos os executáveis gerados na pasta /$(BUILD_DIR)"
	@echo " Para rodar NW Puro (Até tam 15):     ./$(TARGET_PURO) <arq1> <arq2>"
	@echo " Para rodar NW Memoizado (Tam 30+):   ./$(TARGET_MEMO) <arq1> <arq2>"
	@echo " Para rodar NW Dinâmico (Tam 30+):    ./$(TARGET_DYN)  <arq1> <arq2>"
	@echo " Para monitorar tempo e memória:       ./$(TARGET_MONITOR) <executavel> [args...]"
	@echo "================================================================="

# Cria o diretório de build se ele não existir
create_build_dir:
	@mkdir -p $(BUILD_DIR)

# Compilação do Needleman-Wunsch Recursivo Puro
$(TARGET_PURO): $(SRC_DIR)/nw_recursivo_puro.c
	@echo "Compilando NW Recursivo Puro (src/nw_recursivo_puro.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/nw_recursivo_puro.c -o $(TARGET_PURO)

# Compilação do Needleman-Wunsch Recursivo Memoizado
$(TARGET_MEMO): $(SRC_DIR)/nw_recursivo_memoizado.c
	@echo "Compilando NW Recursivo Memoizado (src/nw_recursivo_memoizado.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/nw_recursivo_memoizado.c -o $(TARGET_MEMO)

# Compilação do Needleman-Wunsch com Programação Dinâmica Iterativa
$(TARGET_DYN): $(SRC_DIR)/nw_dinamico.c
	@echo "Compilando NW Programação Dinâmica (src/nw_dinamico.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/nw_dinamico.c -o $(TARGET_DYN)

# Compilação do Monitor de Processos
$(TARGET_MONITOR): $(SRC_DIR)/monitor.c
	@echo "Compilando Monitor (src/monitor.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/monitor.c -o $(TARGET_MONITOR)

# --- Regra de Limpeza ---
clean:
	@echo "Limpando arquivos gerados..."
	rm -rf $(BUILD_DIR)
	@echo "Pronto!"

.PHONY: all create_build_dir clean