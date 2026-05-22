# --- Variáveis de Configuração ---
CC = gcc
CFLAGS = -Wall -Wextra -O3
BUILD_DIR = build
SRC_DIR = src

# Nomes dos executáveis finais dentro da pasta build
TARGET_REC = $(BUILD_DIR)/nw_recursivo
TARGET_DYN = $(BUILD_DIR)/nw_dinamico

# --- Regras Principais ---

# Regra padrão: Compila ambos os códigos
all: create_build_dir $(TARGET_REC) $(TARGET_DYN)
	@echo "================================================="
	@echo " Sucesso! Executáveis gerados na pasta /$(BUILD_DIR)"
	@echo " Para rodar o NW Recursivo: ./$(TARGET_REC)"
	@echo " Para rodar o NW Dinâmico:  ./$(TARGET_DYN)"
	@echo "================================================="

# Cria o diretório de build se ele não existir
create_build_dir:
	@mkdir -p $(BUILD_DIR)

# Compilação do Needleman-Wunsch Recursivo (Buscando na pasta src)
$(TARGET_REC): $(SRC_DIR)/nw_recursivo.c
	@echo "Compilando NW Recursivo (src/nw_recursivo.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/nw_recursivo.c -o $(TARGET_REC)

# Compilação do Needleman-Wunsch com Programação Dinâmica (Buscando na pasta src)
$(TARGET_DYN): $(SRC_DIR)/nw_dinamico.c
	@echo "Compilando NW Programação Dinâmica (src/nw_dinamico.c)..."
	$(CC) $(CFLAGS) $(SRC_DIR)/nw_dinamico.c -o $(TARGET_DYN)

# --- Regra de Limpeza ---
clean:
	@echo "Limpando arquivos gerados..."
	rm -rf $(BUILD_DIR)
	@echo "Pronto!"

# Evita conflitos caso existam arquivos com esses nomes na pasta
.PHONY: all create_build_dir clean