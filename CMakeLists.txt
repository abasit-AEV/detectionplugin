cmake_minimum_required(VERSION 3.14)

project(RecognationLibrary LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(OpenCV REQUIRED )

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(RecognationLibrary SHARED
  RecognationLibrary_global.h
  IRecognation.h
  recognationlibrary.cpp
  recognationlibrary.h
)


target_link_libraries(RecognationLibrary PRIVATE Qt${QT_VERSION_MAJOR}::Core  ${OpenCV_LIBS})

target_compile_definitions(RecognationLibrary PRIVATE RECOGNATIONLIBRARY_LIBRARY)
