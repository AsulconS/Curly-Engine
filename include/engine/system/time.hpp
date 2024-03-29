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

#include <core/config.hpp>
#include <core/common.hpp>

namespace sys
{
/**
 * @brief Timer Class that measures the time intervals, calculate Delta Time and other time stuff
 * 
 */
class Time final
{
    friend constexpr Time seconds(cfg::secT seconds);
    friend constexpr Time milliseconds(cfg::milliT milliseconds);
    friend constexpr Time microseconds(cfg::microT microseconds);
    friend constexpr Time rawTimeBuilder(cfg::microT microseconds);
public:
    /**
     * @brief Construct a new Time object
     * 
     */
    constexpr Time();

    /**
     * @brief Get the Time Point value as seconds
     * 
     * @return secT 
     */
    constexpr cfg::secT asSeconds() const;
    /**
     * @brief Get the Time Point value as milliseconds
     * 
     * @return milliT 
     */
    constexpr cfg::milliT asMilliseconds() const;
    /**
     * @brief Get the Time Point value as microseconds
     * 
     * @return microT 
     */
    constexpr cfg::microT asMicroseconds() const;
    /**
     * @brief Get the Raw Time Count as microseconds
     * It's equivalent to asMicroseconds function
     * 
     * @return microT 
     */
    constexpr cfg::microT getRawTimeCount() const;

private:
    /**
     * @brief Construct a new Time object explicitly
     * in order to prevent outer construction
     * 
     * @param t_timeCount 
     */
    constexpr explicit Time(cfg::microT t_timeCount);

private:
    cfg::microT m_timeCount;
};

/**
 * @brief Construct a new Time object given seconds amount
 * 
 * @param seconds 
 * @return Time 
 */
constexpr Time seconds(cfg::secT seconds);
/**
 * @brief Construct a new Time object given milliseconds amount
 * 
 * @param milliseconds 
 * @return Time 
 */
constexpr Time milliseconds(cfg::milliT milliseconds);
/**
 * @brief Construct a new Time object given microseconds amount
 * 
 * @param microseconds 
 * @return Time 
 */
constexpr Time microseconds(cfg::microT microseconds);

/**
 * @brief Construct a new Time object given microseconds amount
 * It's equivalent to microseconds function
 * 
 * @param microseconds 
 * @return Time 
 */
constexpr Time rawTimeBuilder(cfg::microT microseconds);

/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if equal
 * @return false if different
 */
constexpr bool operator==(const Time& lhs, const Time& rhs);
/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if different
 * @return false if equal
 */
constexpr bool operator!=(const Time& lhs, const Time& rhs);

/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if lhs is less than rhs
 * @return false otherwise
 */
constexpr bool operator<(const Time& lhs, const Time& rhs);
/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if lhs is greater than rhs
 * @return false otherwise
 */
constexpr bool operator>(const Time& lhs, const Time& rhs);
/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if lhs is less or equal than rhs
 * @return false otherwise
 */
constexpr bool operator<=(const Time& lhs, const Time& rhs);
/**
 * @brief Compare two Time Point values
 * 
 * @param lhs 
 * @param rhs 
 * @return true if lhs is greater or equal than rhs
 * @return false otherwise
 */
constexpr bool operator>=(const Time& lhs, const Time& rhs);

/**
 * @brief Changes sign of time
 * 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator-(const Time& rhs);

/**
 * @brief Add two Time Points
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator+(const Time& lhs, const Time& rhs);
/**
 * @brief Subtract two Time Points
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator-(const Time& lhs, const Time& rhs);
/**
 * @brief Multiply a Time Point by a floating point scalar
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator*(const Time& lhs, float rhs);
/**
 * @brief Multiply a Time Point by an integer scalar
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator*(const Time& lhs, cfg::int64 rhs);
/**
 * @brief Multiply a floating point scalar by a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator*(float lhs, const Time& rhs);
/**
 * @brief Multiply an integer scalar by a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator*(cfg::int64 lhs, const Time& rhs);
/**
 * @brief Divide a Time Point by a floating point scalar
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator/(const Time& lhs, float rhs);
/**
 * @brief Divide a Time Point by an integer scalar
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator/(const Time& lhs, cfg::int64 rhs);
/**
 * @brief Divide a floating point scalar by a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator/(float lhs, const Time& rhs);
/**
 * @brief Divide an integer scalar by a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time 
 */
constexpr Time operator/(cfg::int64 lhs, const Time& rhs);

/**
 * @brief Add a Time Point to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator+=(Time& lhs, const Time& rhs);
/**
 * @brief Subtract a Time Point to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator-=(Time& lhs, const Time& rhs);
/**
 * @brief Multiply a floating point scalar to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator*=(Time& lhs, float rhs);
/**
 * @brief Multiply an integer scalar to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator*=(Time& lhs, cfg::int64 rhs);
/**
 * @brief Divide a floating point scalar to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator/=(Time& lhs, float rhs);
/**
 * @brief Divide an integer scalar to a Time Point
 * 
 * @param lhs 
 * @param rhs 
 * @return Time& 
 */
Time& operator/=(Time& lhs, cfg::int64 rhs);

} // namespace sys

#include <system/time.inl>
