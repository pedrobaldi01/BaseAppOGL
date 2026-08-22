cmake_minimum_required(VERSION 3.21)

# Depois que o inicializador do Windows prepara o vcpkg, o CLion e o Visual
# Studio tambem conseguem reutiliza-lo automaticamente ao abrir esta pasta.
if(CMAKE_HOST_WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE
            "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH "Toolchain do vcpkg"
        )
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE
            "${CMAKE_CURRENT_SOURCE_DIR}/.tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH "Toolchain local do vcpkg"
        )
    endif()
endif()

project(BaseAppOGL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Freetype REQUIRED)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.cpp"
)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Scenes/*.h"
)

add_executable(BaseAppOGL
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(BaseAppOGL PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)
    # Faz o MSVC informar corretamente o nivel de C++ e melhora a
    # compatibilidade de recursos como std::format.
    target_compile_options(BaseAppOGL PRIVATE /Zc:__cplusplus)
endif()

target_link_libraries(BaseAppOGL PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
    assimp::assimp
    Freetype::Freetype
)

if(APPLE)
    target_compile_definitions(BaseAppOGL PRIVATE GL_SILENCE_DEPRECATION)
endif()

# Organiza os .cpp e .h no Solution Explorer quando o CMake gera um projeto
# para o Visual Studio.
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

set_target_properties(BaseAppOGL PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Mantem os caminhos relativos validos tanto no CLion quanto ao abrir o
# executavel diretamente pela pasta de build.
add_custom_command(TARGET BaseAppOGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Fonts"
        "$<TARGET_FILE_DIR:BaseAppOGL>/Fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Scenes"
        "$<TARGET_FILE_DIR:BaseAppOGL>/Scenes"
)
