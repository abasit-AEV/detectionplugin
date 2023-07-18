cmake_minimum_required(VERSION 3.14)

project(RecognationTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 CONFIG REQUIRED)
find_package(OpenCV 4.7 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(RecognationLibrary REQUIRED CONFIG)

add_executable(test test.cpp)
target_include_directories(test PRIVATE "D:/uprojects/projects/companies/upwork/brian_moffatt/camview/OpenCVCam/DetectionDLL/buildlib_vs")

#target_include_directories(test PRIVATE ${RecognationLibrary_INCLUDE_DIRS})
target_link_libraries(test PRIVATE pybind11::embed Qt${QT_VERSION_MAJOR}::Core ${OpenCV_LIBS} "D:/uprojects/projects/companies/upwork/brian_moffatt/camview/OpenCVCam/DetectionDLL/buildlib_vs/out/build/x64-Debug/RecognationLibrary.lib")
