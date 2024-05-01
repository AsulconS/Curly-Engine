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

#include "device.hpp"

#include <set>
#include <string>
#include <iostream>

namespace vkUtils
{
//--------------------------------------------------------------------------------
bool QueueFamilyInfo::isFilled() const
{
	return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
}

#if CURLY_DEBUG
//--------------------------------------------------------------------------------
void logDeviceProperties(const vk::PhysicalDevice& device)
{
	/**
	 * from vulkan_structs.hpp
	 *
	 * VULKAN_HPP_CONSTEXPR_14 PhysicalDeviceProperties( uint32_t                                 apiVersion_    = {},
														 uint32_t                                 driverVersion_ = {},
														 uint32_t                                 vendorID_      = {},
														 uint32_t                                 deviceID_      = {},
														 VULKAN_HPP_NAMESPACE::PhysicalDeviceType deviceType_ = VULKAN_HPP_NAMESPACE::PhysicalDeviceType::eOther,
														 std::array<char, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE> const & deviceName_        = {},
														 std::array<uint8_t, VK_UUID_SIZE> const &                  pipelineCacheUUID_ = {},
														 VULKAN_HPP_NAMESPACE::PhysicalDeviceLimits                 limits_            = {},
														 VULKAN_HPP_NAMESPACE::PhysicalDeviceSparseProperties       sparseProperties_  = {} ) VULKAN_HPP_NOEXCEPT
	 */
	vk::PhysicalDeviceProperties properties = device.getProperties();

	// Logging
	std::cout << "\tDevice name: " << properties.deviceName << std::endl;
	const char* deviceType =
		(properties.deviceType == vk::PhysicalDeviceType::eCpu) ? "CPU" :
		(properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) ? "Discrete GPU" :
		(properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) ? "Integrated GPU" :
		(properties.deviceType == vk::PhysicalDeviceType::eVirtualGpu) ? "Virtual GPU" :
		"Other";
	std::cout << "\tDevice type: " << deviceType << std::endl;
}
#endif

//--------------------------------------------------------------------------------
bool checkSupportOnDeviceExtension(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtensions)
{
	// Create set of required extension based on request
	std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());
#if CURLY_DEBUG
	std::cout << "\tDevice Supported Extensions:" << std::endl;
#endif
	for (const vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
	{
#if CURLY_DEBUG
		std::cout << "\t\t" << extension.extensionName << std::endl;
#endif
		// Erase extensions as they appear to satisfy requirements
		requiredExtensions.erase(extension.extensionName);
	}
	// Return support if all requirements are satisfied (empty set)
	return requiredExtensions.empty();
}

//--------------------------------------------------------------------------------
bool isDeviceSuitable(const vk::PhysicalDevice& device)
{
	// Ensure Swapchain support
	const char* rawExtensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	constexpr int rawExtensionsNum = sizeof(rawExtensions) / sizeof(const char*);
	const std::vector<const char*> suitabilityRequestedExtensions(rawExtensions, rawExtensions + rawExtensionsNum);
#if CURLY_DEBUG
	std::cout << "\tDevice Suitability Requested Extensions:" << std::endl;
	for (const char* extensionName : suitabilityRequestedExtensions)
	{
		std::cout << "\t\t" << extensionName << std::endl;
	}
#endif
	const bool isSuitable = checkSupportOnDeviceExtension(device, suitabilityRequestedExtensions);
#if CURLY_DEBUG
	if (isSuitable)
	{
		std::cout << "\tDevice Suitable!" << std::endl;
	}
	else
	{
		std::cout << "\tDevice not Suitable!" << std::endl;
	}
#endif
	return isSuitable;
}

//--------------------------------------------------------------------------------
/**
 * @brief Vulkan separates the concept of physical and logical devices.
 *
 * A physical device usually represents a single complete implementation of Vulkan
 * (excluding instance-level functionality) available to the host, of which there
 * are a finite number.
 *
 * A logical device represents an instance of that implementation with its own state
 * and resources independent of other logical devices.
 *
 * @param instance
 * @return vk::PhysicalDevice
 */
vk::PhysicalDevice choosePhysicalDevice(vk::Instance& instance)
{
	/**
	 * Choose a suitable physical device from a list of candidates.
	 * Note: Physical devices are neither created nor destroyed,
	 * they exist independently to the program.
	 */
	 //

	// Get available devices
	const std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
#if CURLY_DEBUG
	std::cout << std::format("There are {} physical devices available:", availableDevices.size()) << std::endl;
#endif

	// Check suitable devices
	for (vk::PhysicalDevice device : availableDevices)
	{
#if CURLY_DEBUG
		logDeviceProperties(device);
#endif
		if (isDeviceSuitable(device))
		{
			return device;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------------
QueueFamilyInfo findQueueFamilies(vk::PhysicalDevice device)
{
	QueueFamilyInfo info;

	std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
#if CURLY_DEBUG
	std::cout << std::format("Device can support {} queue families.", queueFamilies.size()) << std::endl;
#endif

	for (uint32 i = 0; i < queueFamilies.size(); ++i)
	{
		const vk::QueueFamilyProperties& queueFamily = queueFamilies[i];
		/**
		 *
			// Provided by VK_VERSION_1_0:
			typedef struct VkQueueFamilyProperties {
				VkQueueFlags    queueFlags;
				uint32_t        queueCount;
				uint32_t        timestampValidBits;
				VkExtent3D      minImageTransferGranularity;
			} VkQueueFamilyProperties;

			typedef enum VkQueueFlagBits {
				VK_QUEUE_GRAPHICS_BIT = 0x00000001,
				VK_QUEUE_COMPUTE_BIT = 0x00000002,
				VK_QUEUE_TRANSFER_BIT = 0x00000004,
				VK_QUEUE_SPARSE_BINDING_BIT = 0x00000008,
			// Provided by VK_VERSION_1_1
				VK_QUEUE_PROTECTED_BIT = 0x00000010,
			// Provided by VK_KHR_video_decode_queue
				VK_QUEUE_VIDEO_DECODE_BIT_KHR = 0x00000020,
			// Provided by VK_KHR_video_encode_queue
				VK_QUEUE_VIDEO_ENCODE_BIT_KHR = 0x00000040,
			// Provided by VK_NV_optical_flow
				VK_QUEUE_OPTICAL_FLOW_BIT_NV = 0x00000100,
			} VkQueueFlagBits;
		 */
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			info.graphicsFamilyIndex = i;
			info.presentFamilyIndex = i; // TODO: Check for valid present family (WA)
#if CURLY_DEBUG
			std::cout << std::format("Queue Family Nr. {} is suitable for graphics and presenting.", i) << std::endl;
#endif
		}

		if (info.isFilled())
		{
			break;
		}
	}
	return info;
}

//--------------------------------------------------------------------------------
vk::Device createLogicalDevice(vk::PhysicalDevice physicalDevice)
{
	QueueFamilyInfo info = findQueueFamilies(physicalDevice);
	const float queuePriority = 1.0f;

	/**
	 * from vulkan_structs.hpp
	 *
	 * VULKAN_HPP_CONSTEXPR DeviceQueueCreateInfo( VULKAN_HPP_NAMESPACE::DeviceQueueCreateFlags flags_            = {},
												   uint32_t                                     queueFamilyIndex_ = {},
												   uint32_t                                     queueCount_       = {},
												   const float *                                pQueuePriorities_ = {},
												   const void *                                 pNext_            = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	const vk::DeviceQueueCreateInfo queueCreateInfo(
		vk::DeviceQueueCreateFlags(), info.graphicsFamilyIndex.value(),
		1, &queuePriority
	);

	// Device Features (set inside struct)
	vk::PhysicalDeviceFeatures deviceFeatures;

	/**
	 * from vulkan_structs.hpp
	 *
	 * VULKAN_HPP_CONSTEXPR DeviceCreateInfo( VULKAN_HPP_NAMESPACE::DeviceCreateFlags              flags_                   = {},
											  uint32_t                                             queueCreateInfoCount_    = {},
											  const VULKAN_HPP_NAMESPACE::DeviceQueueCreateInfo *  pQueueCreateInfos_       = {},
											  uint32_t                                             enabledLayerCount_       = {},
											  const char * const *                                 ppEnabledLayerNames_     = {},
											  uint32_t                                             enabledExtensionCount_   = {},
											  const char * const *                                 ppEnabledExtensionNames_ = {},
											  const VULKAN_HPP_NAMESPACE::PhysicalDeviceFeatures * pEnabledFeatures_        = {},
											  const void *                                         pNext_                   = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	std::vector<const char*> enabledLayers;
#if CURLY_DEBUG
	enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif
	const vk::DeviceCreateInfo deviceCreateInfo(
		vk::DeviceCreateFlags(), // Flags
		1, &queueCreateInfo, // Create Info
		static_cast<uint32_t>(enabledLayers.size()), enabledLayers.data(), // Layers
		0, nullptr, // Extensions
		&deviceFeatures // Features
	);

	try
	{
		return physicalDevice.createDevice(deviceCreateInfo);
	}
	catch (const vk::SystemError& e)
	{
		std::cerr << "Failed to create Logical Device!" << std::endl;
		return nullptr;
	}
}

//--------------------------------------------------------------------------------
vk::Queue getQueue(vk::PhysicalDevice physicalDevice, vk::Device device)
{
	QueueFamilyInfo info = findQueueFamilies(physicalDevice);
	return device.getQueue(info.graphicsFamilyIndex.value(), 0);
}

} // namespace vkUtils
