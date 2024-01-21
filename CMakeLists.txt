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

# Project options
option(CURLY_LOCAL_RC "Enable RC File Support for local builds (export icon)" OFF)
option(CURLY_FORCE_GLX_CTX_VERSION OFF)
set(CURLY_GLX_CTX_VERSION_MAJOR 4 CACHE STRING "Specifies Forced GLX Version Major")
set(CURLY_GLX_CTX_VERSION_MINOR 6 CACHE STRING "Specifies Forced GLX Version Minor")

# Set C++20 as the standard
set(CMAKE_CXX_STANDARD 20)

set(CURLY_RUNTIME_LIB_NAME curly-runtime)

# Set Target Platform
if(WIN32)
    set(CURLY_PLATFORM win32)
else()
    set(CURLY_PLATFORM linux)
endif()

# Icon
if(CURLY_LOCAL_RC)
    set(OPT_RES_OBJ "other/res/curly-rc.o")
else()
    set(OPT_RES_OBJ "")
endif()

# Get Source Files
set(CURLY_RUNTIME_SOURCES
    src/engine/core/GL/gl.c
    src/engine/system/timer.cpp
    src/engine/system/${CURLY_PLATFORM}/timerPlatform.cpp
    src/engine/graphics/gUtils.cpp
    src/engine/graphics/mesh.cpp
    src/engine/graphics/model.cpp
    src/engine/graphics/shader.cpp
    src/engine/math/mUtils.cpp
    src/engine/math/vecArithmetic.cpp
    src/engine/window/inputHandler.cpp
    src/engine/window/renderingWindow.cpp
    src/engine/window/${CURLY_PLATFORM}/windowManagerPlatform.cpp
    src/engine/window/${CURLY_PLATFORM}/compatUtilsPlatform.cpp
    src/engine/window/wmLazyPtr.cpp
    src/engine/window/wUtils.cpp
)

# Set Dynamic Compile Definitions
set(CURLY_BUILD_DEFINITIONS)
if(CURLY_FORCE_GLX_CTX_VERSION)
    set(CURLY_BUILD_DEFINITIONS ${CURLY_BUILD_DEFINITIONS}
        C__CURLY_FORCE_GLX_CTX_VERSION
        C__CURLY_GLX_CTX_VERSION_MAJOR=${CURLY_GLX_CTX_VERSION_MAJOR}
        C__CURLY_GLX_CTX_VERSION_MINOR=${CURLY_GLX_CTX_VERSION_MINOR}
    )
endif()

# Build library modules
add_library(${CURLY_RUNTIME_LIB_NAME} SHARED ${CURLY_RUNTIME_SOURCES})
set_target_properties(${CURLY_RUNTIME_LIB_NAME}
PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${SO_VERSION}
)

# Set Compile Definitions
target_compile_definitions(${CURLY_RUNTIME_LIB_NAME}
PUBLIC
    GLAD_API_CALL_EXPORT
    C__CURLY_API_CALL_EXPORT
PRIVATE
    __STDC_LIB_EXT1__
    GLAD_API_CALL_EXPORT_BUILD
    C__CURLY_API_CALL_EXPORT_BUILD
    ${CURLY_BUILD_DEFINITIONS}
)

# Add Include Directories and Link
target_include_directories(${CURLY_RUNTIME_LIB_NAME} PRIVATE include/engine)
if(WIN32)
    target_link_libraries(${CURLY_RUNTIME_LIB_NAME} opengl32)
else()
    target_link_libraries(${CURLY_RUNTIME_LIB_NAME} GL X11)
endif()

# Build main runtime module
add_executable(curly src/editor/app.cpp ${OPT_RES_OBJ})
add_dependencies(curly ${CURLY_RUNTIME_LIB_NAME})
target_compile_definitions(curly
PUBLIC
    GLAD_API_CALL_EXPORT
    C__CURLY_API_CALL_EXPORT
PRIVATE
    __STDC_LIB_EXT1__
)
target_include_directories(curly PRIVATE include)
target_include_directories(curly PRIVATE include/engine)
target_include_directories(curly PRIVATE include/editor)
if(WIN32)
    target_link_libraries(curly LINK_PUBLIC ${CURLY_RUNTIME_LIB_NAME} opengl32)
else()
    target_link_libraries(curly LINK_PUBLIC ${CURLY_RUNTIME_LIB_NAME})
endif()
