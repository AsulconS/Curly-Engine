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

#ifndef WINDOW_PARAMS_HPP
#define WINDOW_PARAMS_HPP

#include <math/vec2.hpp>

#include <window/inputBindings.hpp>

namespace wnd
{

struct WindowParams
{
};

struct MouseParams : public WindowParams
{
    InputCode code;
    math::Vec2i pos;
};

struct KeyboardParams : public WindowParams
{
    InputCode code;
};

struct WindowRectParams : public WindowParams
{
    int clientWidth;
    int clientHeight;
    int windowWidth;
    int windowHeight;
};

} // namespace wnd

#endif // CURLY_WINDOW_PARAMS_HPP