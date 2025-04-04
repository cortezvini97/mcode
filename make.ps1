# Definir os diretórios do projeto
$srcDir = "src"
$includeDir = "include"
$buildDir = "build"
$outputFile = "$buildDir\mcode.exe"

# Função para limpar o diretório de build
function Clean {
    if (Test-Path $buildDir) {
        Write-Host "Limpando o diretório de build..."
        Remove-Item -Recurse -Force $buildDir
        Write-Host "Diretório de build removido."
    } else {
        Write-Host "O diretório de build não existe."
    }
}

# Função para compilar o projeto
function Build {
    # Criar o diretório de build, se não existir
    if (-not (Test-Path $buildDir)) {
        New-Item -ItemType Directory -Force -Path $buildDir
    }

    # Arquivos de código-fonte
    $srcFiles = @(
        "$srcDir\main.c",
        "$srcDir\functions_win.c"
    )

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
}

# Processar os parâmetros passados na linha de comando
if ($args -contains "--clean") {
    Clean
}

if ($args -contains "--build") {
    Build
}

# Caso nenhum parâmetro seja passado, exibe ajuda
if (-not ($args -contains "--build" -or $args -contains "--clean")) {
    Write-Host "Uso: .\make.ps1 --build (para compilar) ou .\make.ps1 --clean (para limpar o diretório de build)"
}