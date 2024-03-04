/**
 * Curly Engine
 * Copyright (c) 2019-2024 Adrian Bedregal
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

/**
 * This defines the OS we are working with
 * Currently: Linux and Windows
 * Details: Deals with dynamic linking semantics
 */
#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)
    #define CF__CURLY_OS_WINDOWS
    #define CF__CURLY_DLL_EXPORT __declspec(dllexport)
    #define CF__CURLY_DLL_IMPORT __declspec(dllimport)
#elif defined(__unix__) || defined(linux) || defined(__GNUC__)
    #define CF__CURLY_OS_LINUX
    #define CF__CURLY_DLL_EXPORT __attribute__((visibility("default")))
    #define CF__CURLY_DLL_IMPORT
#else
    #define CF__CURLY_OS_UNKNOWN
    #define CF__CURLY_DLL_EXPORT
    #define CF__CURLY_DLL_IMPORT
    #pragma warning Unknown semantics for dynamic linking
    #error Curly Engine has no support for this OS
#endif

#if defined(C__CURLY_API_CALL_EXPORT)
    #if defined(C__CURLY_API_CALL_EXPORT_BUILD)
        #define CURLY_API CF__CURLY_DLL_EXPORT
    #else
        #define CURLY_API CF__CURLY_DLL_IMPORT
    #endif
#else
    #define CURLY_API
#endif

/**
 * @brief This enables or disabled VLD based
 * on build type and options
 */
#if !defined(CURLY_RELEASE) && defined(C__CURLY_DEV_OPT_1)
    #include <vld.h>
#endif
