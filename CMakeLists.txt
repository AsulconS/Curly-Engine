cmake_minimum_required(VERSION 3.20)

# Disable CYGWIN Legacy Warning
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Set where out should go
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}>/binaries/$<IF:$<CONFIG:Debug>,Debug,Release>)

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
option(CURLY_DEV_OPT_1 OFF) # VLD on Windows
set(CURLY_GLX_CTX_VERSION_MAJOR 4 CACHE STRING "Specifies Forced GLX Version Major")
set(CURLY_GLX_CTX_VERSION_MINOR 6 CACHE STRING "Specifies Forced GLX Version Minor")

# Set C++20 as the standard
set(CMAKE_CXX_STANDARD 20)

set(CURLY_RUNTIME_LIB_NAME curly-runtime)
set(CURLY_EDITOR_EXEC_NAME curly)

# Set Target Platform
if(WIN32)
    set(CURLY_PLATFORM win32)
else()
    set(CURLY_PLATFORM linux)
endif()

# Icon
if(CURLY_LOCAL_RC)
    set(OPT_RES_FILE "other/res/curly.res")
else()
    set(OPT_RES_FILE "")
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
)
set(CURLY_EDITOR_SOURCES
    src/editor/core/appMainProc.cpp
    src/editor/window/editorWindow.cpp
    src/editor/window/${CURLY_PLATFORM}/windowManagerPlatform.cpp
    src/editor/app.cpp
)

# Set Dynamic Compile Definitions
set(CURLY_RUNTIME_BUILD_DEFINITIONS $<IF:$<CONFIG:Debug>,CURLY_DEBUG,CURLY_RELEASE>)
set(CURLY_EDITOR_BUILD_DEFINITIONS $<IF:$<CONFIG:Debug>,CURLY_DEBUG,CURLY_RELEASE>)
if(CURLY_FORCE_GLX_CTX_VERSION)
    set(CURLY_RUNTIME_BUILD_DEFINITIONS ${CURLY_RUNTIME_BUILD_DEFINITIONS}
        C__CURLY_FORCE_GLX_CTX_VERSION
        C__CURLY_GLX_CTX_VERSION_MAJOR=${CURLY_GLX_CTX_VERSION_MAJOR}
        C__CURLY_GLX_CTX_VERSION_MINOR=${CURLY_GLX_CTX_VERSION_MINOR}
    )
endif()
if (WIN32)
    set(SDK_VERSION ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION})
    string(REPLACE "." ";" SDK_VERSION_AS_LIST ${SDK_VERSION})
    list(GET SDK_VERSION_AS_LIST 0 SDK_MAJOR)
    list(GET SDK_VERSION_AS_LIST 1 SDK_MINOR)
    list(GET SDK_VERSION_AS_LIST 2 SDK_PATCH)
    set(CURLY_EDITOR_BUILD_DEFINITIONS ${CURLY_EDITOR_BUILD_DEFINITIONS}
        C__WIN_SDK_VERSION_MAJOR=${SDK_MAJOR}
        C__WIN_SDK_VERSION_MINOR=${SDK_MINOR}
        C__WIN_SDK_VERSION_PATCH=${SDK_PATCH}
    )
    if(CURLY_DEV_OPT_1)
        set(CURLY_RUNTIME_BUILD_DEFINITIONS ${CURLY_RUNTIME_BUILD_DEFINITIONS}
            C__CURLY_DEV_OPT_1
        )
        set(CURLY_EDITOR_BUILD_DEFINITIONS ${CURLY_EDITOR_BUILD_DEFINITIONS}
            C__CURLY_DEV_OPT_1
        )
    endif()
endif()

# Build runtime module
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
    ${CURLY_RUNTIME_BUILD_DEFINITIONS}
)

# Add Include Directories and Link
target_include_directories(${CURLY_RUNTIME_LIB_NAME} PRIVATE include)
if(WIN32)
    if(CURLY_DEV_OPT_1)
        target_include_directories(${CURLY_RUNTIME_LIB_NAME} PRIVATE "C:/Program Files (x86)/Visual Leak Detector/include")
        target_link_directories(${CURLY_RUNTIME_LIB_NAME} PRIVATE "C:/Program Files (x86)/Visual Leak Detector/lib/Win64")
        target_link_libraries(${CURLY_RUNTIME_LIB_NAME} opengl32 vld)
    else()
        target_link_libraries(${CURLY_RUNTIME_LIB_NAME} opengl32)
    endif()
else()
    target_link_libraries(${CURLY_RUNTIME_LIB_NAME} GL X11)
endif()

# Build main editor module
add_executable(${CURLY_EDITOR_EXEC_NAME} ${CURLY_EDITOR_SOURCES})
add_dependencies(${CURLY_EDITOR_EXEC_NAME} ${CURLY_RUNTIME_LIB_NAME})
target_compile_definitions(${CURLY_EDITOR_EXEC_NAME}
PUBLIC
    GLAD_API_CALL_EXPORT
    C__CURLY_EDITOR_API_CALL_EXPORT
PRIVATE
    __STDC_LIB_EXT1__
    C__CURLY_EDITOR_API_CALL_EXPORT_BUILD
    ${CURLY_EDITOR_BUILD_DEFINITIONS}
)
target_include_directories(${CURLY_EDITOR_EXEC_NAME} PRIVATE include)
if(WIN32)
    if(CURLY_DEV_OPT_1)
        target_include_directories(${CURLY_EDITOR_EXEC_NAME} PRIVATE "C:/Program Files (x86)/Visual Leak Detector/include")
        target_link_directories(${CURLY_EDITOR_EXEC_NAME} PRIVATE "C:/Program Files (x86)/Visual Leak Detector/lib/Win64")
        target_link_libraries(${CURLY_EDITOR_EXEC_NAME} LINK_PUBLIC ${CURLY_RUNTIME_LIB_NAME} opengl32 dwmapi vld)
    else()
        target_link_libraries(${CURLY_EDITOR_EXEC_NAME} LINK_PUBLIC ${CURLY_RUNTIME_LIB_NAME} opengl32 dwmapi)
    endif()
else()
    target_link_libraries(${CURLY_EDITOR_EXEC_NAME} LINK_PUBLIC ${CURLY_RUNTIME_LIB_NAME})
endif()

# Resource link
set(CURLY_LINK_FLAGS ${CMAKE_CURRENT_SOURCE_DIR}/${OPT_RES_FILE})
set_target_properties(${CURLY_EDITOR_EXEC_NAME}
PROPERTIES
    LINK_FLAGS ${CURLY_LINK_FLAGS}
)
