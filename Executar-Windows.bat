@echo off
setlocal

set "PROJECT_DIR=%~dp0"
set "PROJECT_DIR=%PROJECT_DIR:~0,-1%"
set "BUILD_DIR=%PROJECT_DIR%\build\windows-release"
set "BUILD_ONLY=0"

if /I "%~1"=="--build-only" set "BUILD_ONLY=1"

echo == BaseAppOGL: configuracao para Windows ==

where cmake >nul 2>nul
if errorlevel 1 (
    echo CMake nao encontrado.
    echo Instale com: winget install Kitware.CMake
    goto :error
)

where git >nul 2>nul
if errorlevel 1 (
    echo Git nao encontrado.
    echo Instale com: winget install Git.Git
    goto :error
)

if not defined VCPKG_ROOT set "VCPKG_ROOT=%PROJECT_DIR%\.tools\vcpkg"

if not exist "%VCPKG_ROOT%\vcpkg.exe" (
    echo Preparando o gerenciador de dependencias vcpkg...
    if not exist "%VCPKG_ROOT%" (
        git clone https://github.com/microsoft/vcpkg.git "%VCPKG_ROOT%"
        if errorlevel 1 goto :error
    )

    call "%VCPKG_ROOT%\bootstrap-vcpkg.bat" -disableMetrics
    if errorlevel 1 goto :error
)

echo Configurando o projeto e instalando as dependencias...
cmake -S "%PROJECT_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows
if errorlevel 1 (
    echo.
    echo Se o erro mencionar compilador, instale o Visual Studio com a opcao:
    echo "Desenvolvimento para desktop com C++".
    goto :error
)

echo Compilando...
cmake --build "%BUILD_DIR%" --config Release --parallel
if errorlevel 1 goto :error

set "APP="
for /r "%BUILD_DIR%" %%F in (BaseAppOGL.exe) do set "APP=%%F"

if not defined APP (
    echo Executavel BaseAppOGL.exe nao encontrado.
    goto :error
)

echo Compilacao concluida: %APP%

if "%BUILD_ONLY%"=="1" exit /b 0

echo Abrindo BaseAppOGL...
pushd "%PROJECT_DIR%"
"%APP%"
set "APP_EXIT=%ERRORLEVEL%"
popd
exit /b %APP_EXIT%

:error
echo.
echo A configuracao nao foi concluida. Veja a mensagem acima.
if defined CI exit /b 1
pause
exit /b 1
