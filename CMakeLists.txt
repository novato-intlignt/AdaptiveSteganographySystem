cmake_minimum_required(VERSION 3.16)

project(AdaptiveSteganographySystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontrar y vincular las bibliotecas necesarias (Qt, OpenCV y libsndfile)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
find_package(SndFile REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/lib/wavelib/bin/include)

# Especificar la ubicación de la biblioteca
add_library(wavelib STATIC IMPORTED)
set_target_properties(wavelib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/wavelib/bin/lib/libwavelib.a
)
# Fuentes del proyecto
set(PROJECT_SOURCES
    src/main.cpp
    src/controller/stegoController.cpp
    src/controller/stegoController.h
    src/model/cellularAutomaton.cpp
    src/model/cellularAutomaton.h
    src/model/audioProcessor.cpp
    src/model/audioProcessor.h
    src/model/waveletTransform.cpp
    src/model/waveletTransform.h
    src/model/LSBHandler.cpp
    src/model/LSBHandler.h
    src/model/dataProcessor.cpp
    src/model/dataProcessor.h
    src/model/chaosGenerator.cpp
    src/model/chaosGenerator.h
    src/view/mainwindow.cpp
    src/view/mainwindow.h
    src/view/mainwindow.ui
    src/utils/fileHandler.cpp
    src/utils/fileHandler.h
)

# Configuración del ejecutable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AdaptiveSteganographySystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/assets/assets.qrc
    )
else()
    if(ANDROID)
        add_library(AdaptiveSteganographySystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AdaptiveSteganographySystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Vincular bibliotecas al ejecutable
target_link_libraries(AdaptiveSteganographySystem PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    ${OpenCV_LIBS}                  # Vincula OpenCV
    sndfile                         # Vincula libsndfile
    wavelib
)

# Configuración para MacOS/iOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AdaptiveSteganographySystem)
endif()
set_target_properties(AdaptiveSteganographySystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Configuración de instalación
include(GNUInstallDirs)
install(TARGETS AdaptiveSteganographySystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalización de Qt en caso de Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AdaptiveSteganographySystem)
endif()
