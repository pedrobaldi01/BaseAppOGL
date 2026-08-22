# Como configurar e executar o BaseAppOGL

O projeto usa o mesmo `CMakeLists.txt` no macOS e no Windows. Os arquivos de
inicializacao instalam ou restauram as bibliotecas, compilam em modo Release e
abrem o programa com os caminhos de `Fonts` e `Scenes` corretos.

## macOS

1. Abra a pasta do projeto no Finder.
2. Clique duas vezes em `Executar-macOS.command`.
3. Se o macOS bloquear a primeira abertura, clique com o botao direito no
   arquivo, escolha **Abrir** e confirme.

O script confere as ferramentas da Apple e usa o Homebrew para instalar apenas
as dependencias ausentes.

## Windows

Na primeira utilizacao, instale:

- Visual Studio com **Desenvolvimento para desktop com C++**;
- CMake (`winget install Kitware.CMake`);
- Git (`winget install Git.Git`).

Depois, clique duas vezes em `Executar-Windows.bat`. O script baixa o `vcpkg`
para a pasta local `.tools`, instala as bibliotecas descritas em `vcpkg.json`,
compila em Release e abre o programa.

Se quiser trabalhar no Visual Studio depois da primeira configuracao, abra a
solucao gerada dentro de `build/windows-release`. A solucao antiga localizada
na raiz depende de uma configuracao manual chamada `OpenGL_SDK`; o fluxo CMake
gerado pelo inicializador e o recomendado para uma maquina nova.

## CLion

Abra a pasta raiz que contem o `CMakeLists.txt`. O CLion reconhecera os arquivos
`.cpp` e `.h`. No macOS ele pode usar as bibliotecas do Homebrew; no Windows,
execute `Executar-Windows.bat` ao menos uma vez para preparar o `vcpkg`.
Depois disso, use **Reload CMake Project** no CLion para ele detectar o
toolchain local automaticamente.

## Somente compilar

Para validar sem abrir a janela:

```text
./Executar-macOS.command --build-only
Executar-Windows.bat --build-only
```

As pastas `build`, `.tools`, `.idea` e demais arquivos locais estao no
`.gitignore`. Devem ser enviados ao Git o `CMakeLists.txt`, `vcpkg.json`, os dois
inicializadores, o codigo, as fontes, os shaders e os recursos.

## Verificacao automatica no GitHub

O arquivo `.github/workflows/build-multiplataforma.yml` compila o projeto em
macOS e Windows a cada `push` ou pull request. Na pagina do repositorio, o
resultado aparece na aba **Actions**.
