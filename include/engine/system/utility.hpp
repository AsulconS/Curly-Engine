/********************************************************************************
 *                                                                              *
 * Curly Engine                                                                 *
 * Copyright (c) 2021-2024 Adrian Bedregal                                      *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#pragma once

namespace sys
{
template <typename T> struct curly_remove_reference      { typedef T type; };
template <typename T> struct curly_remove_reference<T&>  { typedef T type; };
template <typename T> struct curly_remove_reference<T&&> { typedef T type; };

template <typename T>
inline typename curly_remove_reference<T>::type&& curly_move(T&& t) noexcept { return static_cast<typename curly_remove_reference<T>::type&&>(t); }

template <typename T>
inline T&& curly_forward(typename curly_remove_reference<T>::type& t) noexcept { return static_cast<T&&>(t); }

template <typename T>
inline T&& curly_forward(typename curly_remove_reference<T>::type&& t) noexcept { return static_cast<T&&>(t); }

} // namespace sys
