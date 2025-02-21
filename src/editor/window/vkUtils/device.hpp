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

#include <editor/core/minimal.hpp>

#include <vulkan/vulkan.hpp>

namespace vkUtils
{
/**
 * @brief Queue Family Info Struct for queues references
 *
 */
struct QueueFamilyInfo
{
	optional<uint32> graphicsFamilyIndex;
	optional<uint32> presentFamilyIndex;

	bool isFilled() const;
};

/**
* @brief Chooses Physical Device given an Instance object
*
* @param instance
* @return vk::PhysicalDevice
*/
vk::PhysicalDevice choosePhysicalDevice(vk::Instance& instance);

/**
 * @brief Finds suitable families for certain device
 *
 * @param device
 * @return QueueFamilyInfo
 */
QueueFamilyInfo findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);

/**
 * @brief Create a Logical Device object from physical device
 *
 * @param physicalDevice
 * @return vk::Device
 */
vk::Device createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

/**
 * @brief Get Queue for device
 *
 * @param physicalDevice
 * @param device
 * @return vk::Queue
 */
vk::Queue getQueue(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface);

} // namespace vkUtils
