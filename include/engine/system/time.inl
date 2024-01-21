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

namespace sys
{
inline constexpr Time::Time()
    : m_timeCount {0}
{
}

inline constexpr Time::Time(cfg::microT t_timeCount)
    : m_timeCount {t_timeCount}
{
}

inline constexpr cfg::secT Time::asSeconds() const
{
    return static_cast<cfg::secT>(m_timeCount) / 1000000.0f;
}

inline constexpr cfg::milliT Time::asMilliseconds() const
{
    return static_cast<cfg::milliT>(m_timeCount / 1000);
}

inline constexpr cfg::microT Time::asMicroseconds() const
{
    return m_timeCount;
}

inline constexpr cfg::microT Time::getRawTimeCount() const
{
    return m_timeCount;
}

inline constexpr Time seconds(cfg::secT seconds)
{
    return Time {static_cast<cfg::microT>(seconds * 1000000.0f)};
}

inline constexpr Time milliseconds(cfg::milliT milliseconds)
{
    return Time {static_cast<cfg::microT>(milliseconds * 1000)};
}

inline constexpr Time microseconds(cfg::microT microseconds)
{
    return Time {microseconds};
}

inline constexpr Time rawTimeBuilder(cfg::microT microseconds)
{
    return Time {microseconds};
}

inline constexpr bool operator==(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() == rhs.getRawTimeCount();
}

inline constexpr bool operator!=(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() != rhs.getRawTimeCount();
}

inline constexpr bool operator<(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() < rhs.getRawTimeCount();
}

inline constexpr bool operator>(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() > rhs.getRawTimeCount();
}

inline constexpr bool operator<=(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() <= rhs.getRawTimeCount();
}

inline constexpr bool operator>=(const Time& lhs, const Time& rhs)
{
    return lhs.getRawTimeCount() >= rhs.getRawTimeCount();
}

inline constexpr Time operator-(const Time& rhs)
{
    return rawTimeBuilder(-rhs.getRawTimeCount());
}

inline constexpr Time operator+(const Time& lhs, const Time& rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() + rhs.getRawTimeCount());
}

inline constexpr Time operator-(const Time& lhs, const Time& rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() - rhs.getRawTimeCount());
}

inline constexpr Time operator*(const Time& lhs, float rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() * static_cast<cfg::microT>(rhs));
}

inline constexpr Time operator*(const Time& lhs, cfg::int64 rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() * static_cast<cfg::microT>(rhs));
}

inline constexpr Time operator*(float lhs, const Time& rhs)
{
    return rawTimeBuilder(static_cast<cfg::microT>(lhs) * rhs.getRawTimeCount());
}

inline constexpr Time operator*(cfg::int64 lhs, const Time& rhs)
{
    return rawTimeBuilder(static_cast<cfg::microT>(lhs) * rhs.getRawTimeCount());
}

inline constexpr Time operator/(const Time& lhs, float rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() / static_cast<cfg::microT>(rhs));
}

inline constexpr Time operator/(const Time& lhs, cfg::int64 rhs)
{
    return rawTimeBuilder(lhs.getRawTimeCount() / static_cast<cfg::microT>(rhs));
}

inline constexpr Time operator/(float lhs, const Time& rhs)
{
    return rawTimeBuilder(static_cast<cfg::microT>(lhs) / rhs.getRawTimeCount());
}

inline constexpr Time operator/(cfg::int64 lhs, const Time& rhs)
{
    return rawTimeBuilder(static_cast<cfg::microT>(lhs) / rhs.getRawTimeCount());
}

inline Time& operator+=(Time& lhs, const Time& rhs)
{
    return lhs = lhs + rhs;
}

inline Time& operator-=(Time& lhs, const Time& rhs)
{
    return lhs = lhs - rhs;
}

inline Time& operator*=(Time& lhs, float rhs)
{
    return lhs = lhs * rhs;
}

inline Time& operator*=(Time& lhs, cfg::int64 rhs)
{
    return lhs = lhs * rhs;
}

inline Time& operator/=(Time& lhs, float rhs)
{
    return lhs = lhs / rhs;
}

inline Time& operator/=(Time& lhs, cfg::int64 rhs)
{
    return lhs = lhs / rhs;
}

} // namespace sys
