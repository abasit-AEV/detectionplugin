cmake_minimum_required(VERSION 3.14)

project(RecognationLibrary LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(pybind11 CONFIG REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(OpenCV 4.7 REQUIRED)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Where the library will be installed to" FORCE)

set(public_headers
    IRecognation.h
)

add_library(RecognationLibrary SHARED
    RecognationLibrary_global.h
    recognationlibrary.cpp
    recognationlibrary.h
    ${public_headers}
)

target_link_libraries(RecognationLibrary PRIVATE
    pybind11::embed
    Qt${QT_VERSION_MAJOR}::Core
    ${OpenCV_LIBS}
)

target_compile_definitions(RecognationLibrary PRIVATE RECOGNATIONLIBRARY_LIBRARY)
set_target_properties(RecognationLibrary PROPERTIES PUBLIC_HEADER "${public_headers}")

install(TARGETS RecognationLibrary
    EXPORT RecognationLibraryTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
install(EXPORT RecognationLibraryTargets
    FILE RecognationLibraryTargets.cmake
    NAMESPACE RecognationLibrary::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecognationLibrary
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RecognationLibraryConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/RecognationLibraryConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RecognationLibraryConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecognationLibrary
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RecognationLibraryConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RecognationLibraryConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RecognationLibrary
)
