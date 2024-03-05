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

#include <engine/core/config.hpp>
#include <engine/core/common.hpp>

#include <engine/system/time.hpp>

namespace sys
{
/**
 * @brief Timer Class that measures the time intervals, calculate Delta Time and other time stuff
 * 
 */
class CURLY_API Timer
{
public:
	/**
	 * @brief Construct a new Timer object
	 * 
	 */
	explicit Timer(const bool t_debugMode = false, const float t_period = 1.0f);
	/**
	 * @brief Destroy the Timer object
	 * 
	 */
	virtual ~Timer();

	/**
	 * @brief Update the timer
	 * 
	 */
	void tick();
	/**
	 * @brief Restart the timer
	 * 
	 */
	void restart();

	/**
	 * @brief Get the Delta Time
	 * 
	 * @return float 
	 */
	cfg::secT getDeltaTime();
	/**
	 * @brief Get the Total Frame count
	 * 
	 * @return cfg::uint32 
	 */
	cfg::uint32 getTotalFrames();
	/**
	 * @brief Get the Frames Per Second count
	 * 
	 * @return cfg::uint32 
	 */
	cfg::uint32 getFramesPerSecond();

private:
	cfg::secT procDeltaTime();
	cfg::secT procTotalElapsedTime();
	cfg::secT procCurrentElapsedTime();

	cfg::microT m_start;
	cfg::microT m_currentStart;
	cfg::microT m_lastTime;

	cfg::secT m_deltaTime;
	cfg::secT m_currentTime;
	cfg::uint32 m_totalFrames;
	cfg::uint32 m_framesPerSecond;

	cfg::secT m_period;
	bool m_debugMode;
};

} // namespace sys
