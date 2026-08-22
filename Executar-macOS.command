#!/bin/bash

set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build/macos-release"
BUILD_ONLY=false

if [[ "${1:-}" == "--build-only" ]]; then
    BUILD_ONLY=true
fi

echo "== BaseAppOGL: configuracao para macOS =="

# Arquivos .command abertos pelo Finder nem sempre recebem o PATH configurado
# no terminal. Localiza o Homebrew tanto em Macs Apple Silicon quanto Intel.
if ! command -v brew >/dev/null 2>&1; then
    if [[ -x /opt/homebrew/bin/brew ]]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    elif [[ -x /usr/local/bin/brew ]]; then
        eval "$(/usr/local/bin/brew shellenv)"
    fi
fi

if ! command -v xcrun >/dev/null 2>&1 || ! xcrun --find clang >/dev/null 2>&1; then
    echo "As ferramentas de compilacao da Apple nao estao instaladas."
    echo "Execute: xcode-select --install"
    exit 1
fi

if ! command -v brew >/dev/null 2>&1; then
    echo "O Homebrew nao esta instalado. Instale-o em https://brew.sh e execute este arquivo novamente."
    exit 1
fi

PACKAGES=(cmake pkg-config glfw glew glm assimp freetype)
MISSING=()

for package in "${PACKAGES[@]}"; do
    if ! brew list --versions "$package" >/dev/null 2>&1; then
        MISSING+=("$package")
    fi
done

if (( ${#MISSING[@]} > 0 )); then
    echo "Instalando dependencias ausentes: ${MISSING[*]}"
    brew install "${MISSING[@]}"
else
    echo "Dependencias ja instaladas."
fi

echo "Configurando o projeto..."
cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release

echo "Compilando..."
cmake --build "$BUILD_DIR" --parallel

APP="$BUILD_DIR/BaseAppOGL"
if [[ ! -x "$APP" ]]; then
    echo "Executavel nao encontrado em: $APP"
    exit 1
fi

echo "Compilacao concluida: $APP"

if [[ "$BUILD_ONLY" == false ]]; then
    echo "Abrindo BaseAppOGL..."
    cd "$PROJECT_DIR"
    exec "$APP"
fi
