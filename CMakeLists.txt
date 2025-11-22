cmake_minimum_required(VERSION 4.0)
project(3DINTRODUCTION)

set(CMAKE_CXX_STANDARD 20)

# Configurar SFML usando FetchContent
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)

# Opciones de SFML antes de hacer disponible
set(SFML_BUILD_AUDIO TRUE)
set(SFML_BUILD_GRAPHICS TRUE)
set(SFML_BUILD_WINDOW TRUE)
set(SFML_BUILD_NETWORK FALSE)

FetchContent_MakeAvailable(SFML)

add_executable(3DINTRODUCTION main.cpp)

# Enlazar las librerías de SFML
target_link_libraries(3DINTRODUCTION PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Copiar la carpeta de recursos al directorio de salida
add_custom_command(
        TARGET 3DINTRODUCTION
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src
        $<TARGET_FILE_DIR:3DINTRODUCTION>/src
        COMMENT "Copiando recursos al directorio de salida"
)

# Copiar las DLLs de SFML al directorio de salida (necesario en Windows)
if(WIN32)
    add_custom_command(
            TARGET 3DINTRODUCTION
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            $<TARGET_FILE:sfml-audio>
            $<TARGET_FILE_DIR:3DINTRODUCTION>
            COMMENT "Copiando DLLs de SFML al directorio de salida"
    )

    # Copiar OpenAL DLL para audio
    add_custom_command(
            TARGET 3DINTRODUCTION
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:3DINTRODUCTION>
            COMMENT "Copiando OpenAL DLL"
    )
endif()

