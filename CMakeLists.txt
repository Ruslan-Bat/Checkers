cmake_minimum_required(VERSION 3.20)

project(Checkers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Расположение FLTK (изменить при необходимости)
set(FLTK_ROOT "C:\\FLTK\\bin")
find_package(FLTK REQUIRED)

set(TARGET "Checkers")

# Список исходников
set(SOURCES
    src/main.cpp
    src/Board.cpp
    src/Piece.cpp
    src/Player.cpp
    src/Game.cpp
    src/CheckersWindow.cpp
    src/Graph_lib/Graph.cpp
    src/Graph_lib/GUI.cpp
    src/Graph_lib/Window.cpp
)

# Создание исполняемого файла
add_executable(${TARGET} ${SOURCES})

# Подключаем заголовки
target_include_directories(${TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src  
    ${FLTK_INCLUDE_DIR}
)

# Пути к библиотекам FLTK
target_link_directories(${TARGET} PRIVATE
    ${FLTK_INCLUDE_DIR}/../lib
)

# Линкуем библиотеки FLTK
target_link_libraries(${TARGET} PRIVATE
    ${FLTK_LIBRARIES}
    fltk_jpeg
    fltk_png
    fltk_z
    gdiplus
)

# Копирование ассетов (resources/) в папку сборки
add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${CMAKE_BINARY_DIR}/resources
    COMMENT "Copying resources into runtime output directory"
)

add_dependencies(${TARGET} CopyResources)
