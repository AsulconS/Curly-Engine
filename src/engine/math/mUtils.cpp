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

#include <math/mUtils.hpp>

namespace gil
{
int absolute(int val)
{
    return (val < 0) ? -val : val;
}

float absolute(float val)
{
    return (val < 0.0f) ? -val : val;
}

int npow(int val, unsigned int exp)
{
    int res {1};
    while(exp--)
    {
        res *= val;
    }
    return res;
}

unsigned int npow(unsigned int val, unsigned int exp)
{
    unsigned int res {1};
    while(exp--)
    {
        res *= val;
    }
    return res;
}

float npow(float val, unsigned int exp)
{
    float res {1};
    while(exp--)
    {
        res *= val;
    }
    return res;
}

int clamp(int val, int lBound, int rBound)
{
    if(val < lBound)
    {
        return lBound;
    }
    else if(val > rBound)
    {
        return rBound;
    }
    else
    {
        return val;
    }
}

unsigned int clamp(unsigned int val, unsigned int lBound, unsigned int rBound)
{
    if(val < lBound)
    {
        return lBound;
    }
    else if(val > rBound)
    {
        return rBound;
    }
    else
    {
        return val;
    }
}

float clamp(float val, float lBound, float rBound)
{
    if(val < lBound)
    {
        return lBound;
    }
    else if(val > rBound)
    {
        return rBound;
    }
    else
    {
        return val;
    }
}

bool isBetween(int val, int lBound, int rBound)
{
    return ((val - lBound) * (rBound - val)) >= 0;
}

bool isBetween(unsigned int val, unsigned int lBound, unsigned int rBound)
{
    return ((static_cast<int>(val) - static_cast<int>(lBound)) * (static_cast<int>(rBound) - static_cast<int>(val))) >= 0;
}

bool isBetween(float val, float lBound, float rBound)
{
    return ((val - lBound) * (rBound - val)) >= 0.0f;
}

} // namespace gil
