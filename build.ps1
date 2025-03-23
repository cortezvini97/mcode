# Definir os diretórios do projeto
$srcDir = "src"
$includeDir = "include"
$buildDir = "build"

# Criar o diretório de build, se não existir
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Force -Path $buildDir
}

# Arquivos de código-fonte
$srcFiles = @(
    "$srcDir\main.c",
    "$srcDir\functions.c"
)

# Nome do arquivo executável de saída
$outputFile = "$buildDir\mcode.exe"

# Definir os parâmetros do compilador
$compiler = "gcc"
$compilerFlags = "-Wall -Wextra -g"  # Flags de compilação, pode adicionar outras conforme necessário

# Adicionar o diretório de include ao comando de compilação
$includeFlag = "-I$includeDir"  # Diretório de cabeçalhos

# Compilar o código-fonte
Write-Host "Compilando o projeto..."

$compileCommand = "$compiler $compilerFlags $includeFlag $srcFiles -o $outputFile"
Invoke-Expression $compileCommand

# Verificar se a compilação foi bem-sucedida
if (Test-Path $outputFile) {
    Write-Host "Compilação bem-sucedida! O executável está em: $outputFile"
} else {
    Write-Host "Erro na compilação."
}
