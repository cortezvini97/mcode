

#ifdef _WIN32

#include "functions_win.h"

int main(int argc, char const *argv[]){
    UINT CPAGE_UTF8 = 65001;
  	UINT CPAGE_DEFAULT = GetConsoleOutputCP();
    SetConsoleOutputCP(CPAGE_UTF8);

    validate_args(argc);

    if (argc < 2) {
        return 1;  // Retorna um código de erro
    }

    if (strcmp(argv[1], "--machinecode") == 0) {
        int created = createBinaryFile(argc, argv);
        if(created == 1){
            printf("Arquivo Criado");
        }
    }

    if(strcmp(argv[1], "--machinecodemap") == 0){
        int created = machineCodeMap(argc, argv);
        if(created == 1){
            printf("Arquivo map Criado");
        }
    }

    if (strcmp(argv[1], "--applyModdify") == 0) {
        int created = generateBinaryExecutableFile(argc, argv);
        if(created == 1){
            printf("Arquivo Executável Criado");
        }
    }

    if (strcmp(argv[1], "--findBinary") == 0) {
        int found = findBinary(argc, argv);
        if (found == 1) {
            printf("Sequência binária encontrada e decodificada.");
        }
    }

    if (strcmp(argv[1], "--replace") == 0) {
        int found = replaceBinary(argc, argv);
        if (found == 1) {
            printf("Binário atualizado.");
        }
    }

    SetConsoleOutputCP(CPAGE_DEFAULT);
    

    return 0;
}

#else

#include "functions_linux.h"

int main(int argc, char const *argv[]){
    validate_args(argc);

    if (argc < 2) {
        return 1;  // Retorna um código de erro
    }

    if (strcmp(argv[1], "--machinecode") == 0) {
        int created = createBinaryFile(argc, argv);
        if(created == 1){
            printf("Arquivo Criado\n");
        }
    }

    if(strcmp(argv[1], "--machinecodemap") == 0){
        int created = machineCodeMap(argc, argv);
        if(created == 1){
            printf("Arquivo map Criado\n");
        }
    }

    if (strcmp(argv[1], "--applyModdify") == 0) {
        int created = generateBinaryExecutableFile(argc, argv);
        if(created == 1){
            printf("Arquivo Executável Criado\n");
        }
    }

    if (strcmp(argv[1], "--findBinary") == 0) {
        int found = findBinary(argc, argv);
        if (found == 1) {
            printf("Sequência binária encontrada e decodificada.\n");
        }
    }

    if (strcmp(argv[1], "--replace") == 0) {
        int found = replaceBinary(argc, argv);
        if (found == 1) {
            printf("Binário atualizado.");
        }
    }

    return 0;
}
#endif