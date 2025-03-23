#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <Windows.h>

// Função de validação dos argumentos
void validate_args(int argc);

// Função que verifica a existência de um arquivo
int file_exists(const char *filename);

// Funções para manipulação de arquivos binários e mapeamento de código
int createBinaryFile(int argc, char const *argv[]);
int machineCodeMap(int argc, char const *argv[]);
int generateBinaryExecutableFile(int argc, char const *argv[]);
int findBinary(int argc, char const *argv[]);
int replaceBinary(int argc, char const *argv[]);

#endif // FUNCTIONS_H
