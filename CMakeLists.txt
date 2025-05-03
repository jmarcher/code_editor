cmake_minimum_required(VERSION 3.16)
project(code_editor VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/editor.cpp
    src/fileexplorer.cpp
    src/syntaxhighlighter.cpp
    src/codecompleter.cpp
)

set(HEADERS
    src/mainwindow.h
    src/editor.h
    src/fileexplorer.h
    src/syntaxhighlighter.h
    src/codecompleter.h
)

add_executable(code_editor ${SOURCES} ${HEADERS})
target_link_libraries(code_editor PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Only add tests if GTest is found
find_package(GTest QUIET)
if(GTest_FOUND)
    add_subdirectory(tests)
    enable_testing()
endif() 