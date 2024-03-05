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

//#include <engine/system/dstr/map.hpp>
#include <map>
#define Map std::map

#include <engine/math/vec2.hpp>

#include <engine/window/inputEvents.hpp>
#include <engine/window/inputBindings.hpp>

namespace wnd
{
/**
 * @brief InputHandler class that handles input
 * 
 */
class CURLY_API InputHandler
{
public:
	InputHandler();
	virtual ~InputHandler();

	bool onKeyDown(InputCode key);
	bool onKeyUp(InputCode key);
	bool onKeyReleased(InputCode key);
	bool onKeyTriggered(InputCode key);

	bool onClick(InputCode button);
	bool onRelease(InputCode button);
	bool onButtonDown(InputCode button);
	bool onButtonUp(InputCode button);

	math::Vec2i getMousePos();

	void _tick();

	void _updateKeyEvent(InputCode key, InputEvent event);
	void _updateMouseEvent(InputCode button, InputEvent event);
	void _updateMousePosition(math::Vec2i position);

private:
	struct KeyInfo
	{
		InputEvent event;
		cfg::int32 time;
	};

	struct MouseInfo
	{
		InputEvent event;
		cfg::int32 time;
	};

private:
	void initKey(InputCode key);
	void initButton(InputCode button);

	math::Vec2i m_mousePos;
	Map<InputCode, KeyInfo>* m_keys;
	Map<InputCode, MouseInfo>* m_mouseButtons;

	cfg::int32 m_currentTime;
};

} // namespace wnd
