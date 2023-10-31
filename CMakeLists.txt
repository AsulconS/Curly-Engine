cmake_minimum_required(VERSION 3.20)

# Disable CYGWIN Legacy Warning
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Set where out should go
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>)

# Set all version configuration
set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(SO_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
set(FULL_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Project info
project(CurlyEngine VERSION ${FULL_VERSION})

macro(build_cpp_source outFilename inFilename)
    add_executable(${outFilename} src/${inFilename}.cpp)
endmacro(build_cpp_source)

build_cpp_source(curly main)
