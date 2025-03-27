# Definir os diretórios do projeto
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Criar o diretório de build, se não existir
$(shell mkdir -p $(BUILD_DIR))

# Arquivos de código-fonte
SRC_FILES = $(SRC_DIR)/main.c $(SRC_DIR)/functions_linux.c

# Nome do arquivo executável de saída
OUTPUT_FILE = $(BUILD_DIR)/mcode

# Definir os parâmetros do compilador
CC = gcc
CFLAGS = -Wall -Wextra -g  # Flags de compilação
INCLUDE_FLAG = -I$(INCLUDE_DIR)  # Diretório de cabeçalhos

# Regra padrão para compilar o projeto
all: $(OUTPUT_FILE)

# Como gerar o arquivo executável
$(OUTPUT_FILE): $(SRC_FILES)
	@echo "Compilando o projeto..."
	$(CC) $(CFLAGS) $(INCLUDE_FLAG) $(SRC_FILES) -o $(OUTPUT_FILE)

# Limpeza (opcional)
clean:
	rm -rf $(BUILD_DIR)

# Regra para rodar o executável
run: $(OUTPUT_FILE)
	@echo "Executando o projeto..."
	./$(OUTPUT_FILE)

# Regra para instalar o binário no /usr/local/bin
install: $(OUTPUT_FILE)
	@echo "Instalando o projeto..."
	sudo cp $(OUTPUT_FILE) /usr/local/bin/

unistall: $(OUTPUT_FILE)
	@echo "Desinstalando o projeto..."
	sudo rm -rf /usr/local/bin/mcode
