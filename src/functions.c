#include "functions.h"

void validate_args(int argc){
    if (argc < 2) {
        printf("mcode v1.0.0\n");
        printf("mcode --machinecode <file.exe>\n");
        printf("mcode --machinecodemap <file.exe>\n");
        printf("mcode --applyModdify <file.binary> <file_output>\n");
        printf("mcode --findBinary <file.binary> <binario>\n");
        printf("mcode --replace <file.binary> <binario_old> <binario_new>\n");
    }
}


int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}


int createBinaryFile(int argc, char const *argv[]){
    if(argc < 3){
        printf("Uso: mcode --machinecode <file.exe>");
        return 0;
    }

    const char *file_exe = argv[2];
    char output_file[256];

    
    if (!file_exists(file_exe)) {
        printf("Erro: Arquivo não existe.\n");
        return 0;
    }
    
    char *dot = strrchr(file_exe, '.');
    char file_name[256];
    
    if (dot) {
        size_t len = dot - file_exe;
        strncpy(file_name, file_exe, len);
        file_name[len] = '\0';
    } else {
        strcpy(file_name, file_exe);
    }
    
    snprintf(output_file, sizeof(output_file), "%s.binary", file_name);

    // Abrir arquivo binário para leitura
    FILE *binary_file = fopen(file_exe, "rb");
    if (!binary_file) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return 0;
    }

    // Abrir arquivo de saída para escrita
    FILE *machine_code_file = fopen(output_file, "w");
    if (!machine_code_file) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(binary_file);
        return 0;
    }

    // Ler os bytes do arquivo e escrever a representação binária
    unsigned char byte;
    while (fread(&byte, sizeof(byte), 1, binary_file) == 1) {
        for (int i = 7; i >= 0; i--) {
            fputc((byte & (1 << i)) ? '1' : '0', machine_code_file);
        }
        fputc(' ', machine_code_file);  // Adicionar espaço entre os bytes
    }

    fclose(binary_file);
    fclose(machine_code_file);

    printf("Arquivo %s criado com sucesso!\n", output_file);
    return 1;
}


int machineCodeMap(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Uso: mcode --machinecodemap <file.exe>\n");
        return 0;
    }

    const char *file_exe = argv[2];
    if (!file_exists(file_exe)) {
        printf("Erro: Arquivo não existe.\n");
        return 0;
    }

    // Obtendo o nome do arquivo sem extensão
    char file_name[256];
    strcpy(file_name, file_exe);
    char *dot = strrchr(file_name, '.');
    if (dot) {
        *dot = '\0'; // Remove a extensão
    }

    // Montando comando para executar `xxd`
    char command[512];
    snprintf(command, sizeof(command), "xxd -g 1 %s", file_exe);

    // Executando `xxd` e capturando saída
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        printf("Erro ao executar o comando.\n");
        return 0;
    }

    // Criando arquivo para salvar a saída
    char output_file[512];
    snprintf(output_file, sizeof(output_file), "%s_machine_code_hex.txt", file_name);
    FILE *output = fopen(output_file, "w");
    if (!output) {
        printf("Erro ao criar arquivo de saída.\n");
        pclose(fp);
        return 0;
    }

    // Escrevendo a saída de `xxd` no arquivo
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        fputs(buffer, output);
    }

    // Fechando arquivos
    fclose(output);
    pclose(fp);

    printf("Arquivo gerado: %s\n", output_file);
    return 1;
}


int generateBinaryExecutableFile(int argc, char const *argv[]) {
    if (argc < 4) {
        printf("Uso: mcode --machinecode <file.binary> <file_output>\n");
        return 0;
    }

    const char *file_binary = argv[2];
    const char *file_output = argv[3];

    if (!file_exists(file_binary)) {
        printf("Erro: Arquivo não existe.\n");
        return 0;
    }

    // Abrir o arquivo binário
    FILE *binary_file = fopen(file_binary, "r");
    if (!binary_file) {
        printf("Erro ao abrir o arquivo %s\n", file_binary);
        return 0;
    }

    // Ler o conteúdo do arquivo
    fseek(binary_file, 0, SEEK_END);
    long file_size = ftell(binary_file);
    fseek(binary_file, 0, SEEK_SET);

    char *machine_code = (char *)malloc(file_size + 1);
    if (machine_code == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(binary_file);
        return 0;
    }

    fread(machine_code, 1, file_size, binary_file);
    machine_code[file_size] = '\0'; // Garantir que a string termine corretamente
    fclose(binary_file);

    // Remover espaços
    char *cleaned_machine_code = (char *)malloc(file_size + 1);
    int j = 0;
    for (int i = 0; i < file_size; i++) {
        if (machine_code[i] != ' ') {
            cleaned_machine_code[j++] = machine_code[i];
        }
    }
    cleaned_machine_code[j] = '\0';
    free(machine_code);

    // Converter a string binária para bytes
    int num_bytes = strlen(cleaned_machine_code) / 8;
    unsigned char *byte_list = (unsigned char *)malloc(num_bytes);
    if (byte_list == NULL) {
        printf("Erro ao alocar memória para os bytes\n");
        free(cleaned_machine_code);
        return 0;
    }

    for (int i = 0; i < num_bytes; i++) {
        char byte_str[9] = {0};  // Para armazenar 8 bits + \0
        strncpy(byte_str, cleaned_machine_code + i * 8, 8);
        byte_list[i] = (unsigned char)strtol(byte_str, NULL, 2);
    }
    free(cleaned_machine_code);

    // Criar o arquivo de saída
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%s.exe", file_output);

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        printf("Erro ao abrir o arquivo de saída %s\n", output_filename);
        free(byte_list);
        return 0;
    }

    // Escrever os bytes no arquivo
    fwrite(byte_list, 1, num_bytes, output_file);
    fclose(output_file);

    free(byte_list);
    printf("Arquivo %s criado com sucesso.\n", output_filename);

    return 1;
}

int findBinary(int argc, char const *argv[]) {
    if (argc < 4) {
        printf("Uso: mcode --findBinary <file.binary> <binario>\n");
        return 0;
    }

    const char *file_binary = argv[2];
    const char *binary_search = argv[3];

    // Verifica se o arquivo existe
    if (!file_exists(file_binary)) {
        printf("Erro: Arquivo não existe.\n");
        return 0;
    }

    // Abrir o arquivo binário
    FILE *binary_file = fopen(file_binary, "r");
    if (!binary_file) {
        printf("Erro ao abrir o arquivo %s\n", file_binary);
        return 0;
    }

    // Ler o arquivo binário
    fseek(binary_file, 0, SEEK_END);
    long file_size = ftell(binary_file);
    fseek(binary_file, 0, SEEK_SET);

    char *binary_content = (char *)malloc(file_size + 1);
    if (binary_content == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(binary_file);
        return 0;
    }

    fread(binary_content, 1, file_size, binary_file);
    binary_content[file_size] = '\0'; // Garantir que a string termine corretamente
    fclose(binary_file);

    // Adicionar espaços à sequência binária fornecida
    size_t len = strlen(binary_search);
    if (len % 8 != 0) {
        printf("Erro: A sequência binária fornecida não é válida (não múltipla de 8 bits).\n");
        free(binary_content);
        return 0;
    }

    // Converter a sequência contínua de binário para com espaços
    char formatted_binary[1024] = {0};
    for (size_t i = 0, j = 0; i < len; i += 8, j++) {
        strncat(formatted_binary, binary_search + i, 8);
        if (j < (len / 8) - 1) {
            strncat(formatted_binary, " ", 1); // Adicionar espaço entre os bytes
        }
    }

    // Buscar a sequência binária formatada no conteúdo do arquivo
    if (strstr(binary_content, formatted_binary)) {
        // Decodificar a sequência binária encontrada para texto
        int num_bytes = len / 8;
        unsigned char *decoded_bytes = (unsigned char *)malloc(num_bytes);
        if (decoded_bytes == NULL) {
            printf("Erro ao alocar memória para decodificação.\n");
            free(binary_content);
            return 0;
        }

        // Decodificar a sequência binária
        for (int i = 0; i < num_bytes; i++) {
            char byte_str[9] = {0};  // Para armazenar 8 bits + \0
            strncpy(byte_str, binary_search + i * 8, 8);
            decoded_bytes[i] = (unsigned char)strtol(byte_str, NULL, 2);
        }

        // Imprimir a sequência decodificada
        printf("Sequência binária encontrada e decodificada: ");
        for (int i = 0; i < num_bytes; i++) {
            printf("%c", decoded_bytes[i]);
        }
        printf("\n");

        free(decoded_bytes);
    } else {
        printf("Sequência binária não encontrada no arquivo.\n");
    }

    free(binary_content);
    return 1;
}


int replaceBinary(int argc, char const *argv[]) {
    if (argc < 5) {
        printf("Uso: mcode --replace <file.binary> <old_binary> <new_binary>\n");
        return 0;
    }

    const char *file_binary = argv[2];
    const char *binary_old = argv[3];
    const char *binary_new = argv[4];

    // Verifica se o arquivo existe
    if (!file_exists(file_binary)) {
        printf("Erro: Arquivo não existe.\n");
        return 0;
    }

    // Abrir o arquivo binário para leitura
    FILE *binary_file = fopen(file_binary, "rb");
    if (!binary_file) {
        printf("Erro ao abrir o arquivo %s\n", file_binary);
        return 0;
    }

    // Ler o arquivo binário
    fseek(binary_file, 0, SEEK_END);
    long file_size = ftell(binary_file);
    fseek(binary_file, 0, SEEK_SET);

    char *binary_content = (char *)malloc(file_size);
    if (binary_content == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(binary_file);
        return 0;
    }

    fread(binary_content, 1, file_size, binary_file);
    fclose(binary_file);

    // Verifica se a sequência binária antiga é válida (múltiplo de 8 bits)
    size_t len_old = strlen(binary_old);
    if (len_old % 8 != 0) {
        printf("Erro: A sequência binária fornecida não é válida (não múltipla de 8 bits).\n");
        free(binary_content);
        return 0;
    }

    // Adicionar espaços à sequência binária antiga
    char formatted_binary_old[1024] = {0};
    for (size_t i = 0, j = 0; i < len_old; i += 8, j++) {
        strncat(formatted_binary_old, binary_old + i, 8);
        if (j < (len_old / 8) - 1) {
            strncat(formatted_binary_old, " ", 1); // Adicionar espaço entre os bytes
        }
    }

    // Verifica se a sequência binária nova é válida (múltiplo de 8 bits)
    size_t len_new = strlen(binary_new);
    if (len_new % 8 != 0) {
        printf("Erro: A sequência binária nova fornecida não é válida (não múltipla de 8 bits).\n");
        free(binary_content);
        return 0;
    }

    // Adicionar espaços à sequência binária nova
    char formatted_binary_new[1024] = {0};
    for (size_t i = 0, j = 0; i < len_new; i += 8, j++) {
        strncat(formatted_binary_new, binary_new + i, 8);
        if (j < (len_new / 8) - 1) {
            strncat(formatted_binary_new, " ", 1); // Adicionar espaço entre os bytes
        }
    }

    // Substituir a sequência binária antiga pela nova no conteúdo
    char *pos = binary_content;
    while ((pos = strstr(pos, formatted_binary_old)) != NULL) {
        size_t old_len = strlen(formatted_binary_old);
        size_t new_len = strlen(formatted_binary_new);

        // Verifica se a nova sequência é maior ou menor que a antiga
        if (new_len > old_len) {
            // Realocar o espaço para o novo conteúdo, se necessário
            binary_content = realloc(binary_content, file_size + (new_len - old_len));
            file_size += (new_len - old_len);
        } else if (new_len < old_len) {
            // Realocar o espaço para o novo conteúdo, se necessário
            binary_content = realloc(binary_content, file_size - (old_len - new_len));
            file_size -= (old_len - new_len);
        }

        // Substituir a sequência
        memmove(pos + new_len, pos + old_len, file_size - (pos - binary_content) - old_len);
        memcpy(pos, formatted_binary_new, new_len);
        pos += new_len; // Avançar para o próximo lugar
    }

    // Abrir o arquivo binário para escrita
    binary_file = fopen(file_binary, "wb");
    if (!binary_file) {
        printf("Erro ao abrir o arquivo para escrita %s\n", file_binary);
        free(binary_content);
        return 0;
    }

    // Escrever o conteúdo modificado de volta no arquivo
    fwrite(binary_content, 1, file_size, binary_file);
    fclose(binary_file);

    free(binary_content);

    printf("Substituição realizada com sucesso!\n");
    return 1;
}