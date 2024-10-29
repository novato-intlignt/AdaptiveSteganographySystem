cmake_minimum_required(VERSION 3.10)
project(SecureAudioEmbedding)

set(CMAKE_CXX_STANDARD 17)

# Incluir las rutas de archivos
include_directories(include src/model src/controller src/view src/utils)

# Buscar las bibliotecas de Qt necesarias
#find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Buscar las bibliotecas de OpenCV
#find_package(OpenCV REQUIRED)

# Buscar Eigen
#find_package(Eigen3 REQUIRED)

# Archivos fuente
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/controller/*.cpp
    src/model/*.cpp
    src/view/*.cpp
    src/utils/*.cpp
)

# Crear el ejecutable
add_executable(SecureAudioEmbedding ${SOURCES})

# Enlazar Qt, OpenCV y Eigen
target_link_libraries(SecureAudioEmbedding Qt5::Widgets ${OpenCV_LIBS} Eigen3::Eigen)

# Configuración para archivos de recursos y UI de Qt
qt5_wrap_ui(UI_HEADERS src/view/MainWindow.ui)

# Incluir los encabezados generados por Qt
target_include_directories(SecureAudioEmbedding PRIVATE ${CMAKE_BINARY_DIR})
