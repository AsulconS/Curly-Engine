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

#include "instance.hpp"

#include "../editorWindowManager.hpp"

#include <vector>
#include <cstring>
#include <iostream>
#include <type_traits>

#ifndef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
#endif

namespace vkUtils
{
//--------------------------------------------------------------------------------
template <typename PropertiesType>
bool checkSupportOnNames(const std::vector<const char*>& names)
{
	// Statically check on properties type for enumerating them
	const char* nameQualifier = "Name";
	std::vector<PropertiesType> supportedProperties;
	constexpr bool isLayerName = std::is_same<PropertiesType, vk::LayerProperties>::value;
	constexpr bool isExtensionName = std::is_same<PropertiesType, vk::ExtensionProperties>::value;
	if constexpr (isLayerName)
	{
		nameQualifier = "Layer";
		supportedProperties = vk::enumerateInstanceLayerProperties();
	}
	else if constexpr (isExtensionName)
	{
		nameQualifier = "Extension";
		supportedProperties = vk::enumerateInstanceExtensionProperties();
	}

#if CURLY_DEBUG
	// List and print all supported names on debug
	std::cout << std::format("Supported {}s:", nameQualifier) << std::endl;
	for (const PropertiesType& properties : supportedProperties)
	{
		if constexpr (isLayerName)
		{
			std::cout << '\t' << properties.layerName << std::endl;
		}
		else if constexpr (isExtensionName)
		{
			std::cout << '\t' << properties.extensionName << std::endl;
		}
	}
#endif

	// Actual phase for compatibility check
	bool nameFound = false;
	for (const char* name : names)
	{
		nameFound = false;
		for (const PropertiesType& properties : supportedProperties)
		{
			const char* internalName = "";
			if constexpr (isLayerName)
			{
				internalName = properties.layerName;
			}
			else if constexpr (isExtensionName)
			{
				internalName = properties.extensionName;
			}

			if (strcmp(name, internalName) == 0)
			{
				nameFound = true;
#if CURLY_DEBUG
				std::cout << std::format("{} {} is supported", nameQualifier, name) << std::endl;
#endif
				break;
			}
		}
		if (!nameFound)
		{
#if CURLY_DEBUG
			std::cout << std::format("{} {} is not supported", nameQualifier, name) << std::endl;
#endif
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------------------------
bool areExtensionsAndLayersSupported(const std::vector<const char*>& extensions, const std::vector<const char*>& layers)
{
	const bool areExtensionsSupported = checkSupportOnNames<vk::ExtensionProperties>(extensions);
	const bool areLayersSupported = checkSupportOnNames<vk::LayerProperties>(layers);
	return areExtensionsSupported && areLayersSupported;
}

//--------------------------------------------------------------------------------
vk::Instance createInstance(const char* applicationName)
{
	// Version Handling
	/**
	 * Request earlier version to ensure compatibility
	 * VK_MAKE_API_VERSION(variant, major, minor, patch)
	 */
	uint32_t version = VK_MAKE_API_VERSION(0, 1, 2, 0);
#if CURLY_DEBUG
	std::cout << "---------- Vulkan Logging ----------" << std::endl;
	std::cout << std::format("Vulkan Target Version: {} {}.{}.{}",
		VK_API_VERSION_VARIANT(version),
		VK_API_VERSION_MAJOR(version),
		VK_API_VERSION_MINOR(version),
		VK_API_VERSION_PATCH(version)) << std::endl;
#endif

	// Create App Info from version and name
	/**
	 * from vulkan_structs.hpp:
	 *
	 * VULKAN_HPP_CONSTEXPR ApplicationInfo( const char * pApplicationName_   = {},
											 uint32_t     applicationVersion_ = {},
											 const char * pEngineName_        = {},
											 uint32_t     engineVersion_      = {},
											 uint32_t     apiVersion_         = {},
											 const void * pNext_              = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	const vk::ApplicationInfo appInfo(
		applicationName,
		version,
		"Curly Engine",
		version,
		version
	);

	// Handle Extensions
	const char* rawExtensions[] = {
		VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME
	};
	constexpr int rawExtensionsNum = sizeof(rawExtensions) / sizeof(const char*);
	std::vector<const char*> extensions(rawExtensions, rawExtensions + rawExtensionsNum);
#if CURLY_DEBUG
	extensions.push_back("VK_EXT_debug_utils");
	std::cout << "Requested Extensions:" << std::endl;
	for (const char* extensionName : extensions)
	{
		std::cout << '\t' << extensionName << std::endl;
	}
#endif

	// Handle Layers
	std::vector<const char*> layers;
#if CURLY_DEBUG
	layers.push_back("VK_LAYER_KHRONOS_validation");
	std::cout << "Requested Layers:" << std::endl;
	for (const char* layerName : layers)
	{
		std::cout << '\t' << layerName << std::endl;
	}
#endif

	// Validate extensions and layers
	if (!areExtensionsAndLayersSupported(extensions, layers))
	{
		return nullptr;
	}

	// Create Info for extensions and layers
	/**
	 * from vulkan_structs.hpp:
	 *
	 * VULKAN_HPP_CONSTEXPR InstanceCreateInfo( VULKAN_HPP_NAMESPACE::InstanceCreateFlags     flags_                   = {},
												const VULKAN_HPP_NAMESPACE::ApplicationInfo * pApplicationInfo_        = {},
												uint32_t                                      enabledLayerCount_       = {},
												const char * const *                          ppEnabledLayerNames_     = {},
												uint32_t                                      enabledExtensionCount_   = {},
												const char * const *                          ppEnabledExtensionNames_ = {},
												const void *                                  pNext_                   = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	const vk::InstanceCreateInfo createInfo(
		vk::InstanceCreateFlags(), // Flags
		&appInfo, // App Info
		static_cast<uint32_t>(layers.size()), layers.data(), // Layers
		static_cast<uint32_t>(extensions.size()), extensions.data() // Extensions
	);

	// Instance creation
	try
	{
		return vk::createInstance(createInfo);
	}
	catch (const vk::SystemError& err)
	{
		std::cerr << "Failed to create vk instance" << std::endl;
		return nullptr;
	}
}

//--------------------------------------------------------------------------------
/**
 * @brief Internal Debug Callback Function
 *
 * @param messageSeverity the severity type of message being received:
 * 		typedef enum VkDebugUtilsMessageSeverityFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageSeverityFlagBitsEXT;
 *
 * @param messageType the general type of message being received:
 * 		typedef enum VkDebugUtilsMessageTypeFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = 0x00000002,
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = 0x00000004,
			VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT = 0x00000008,
			VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageTypeFlagBitsEXT;
 *
 * @param pCallbackData contains explicit and more detailed info about the message:
 * 		typedef struct VkDebugUtilsMessengerCallbackDataEXT {
			VkStructureType                              sType;
			const void*                                  pNext;
			VkDebugUtilsMessengerCallbackDataFlagsEXT    flags;
			const char*                                  pMessageIdName;
			int32_t                                      messageIdNumber;
			const char*                                  pMessage;
			uint32_t                                     queueLabelCount;
			const VkDebugUtilsLabelEXT*                  pQueueLabels;
			uint32_t                                     cmdBufLabelCount;
			const VkDebugUtilsLabelEXT*                  pCmdBufLabels;
			uint32_t                                     objectCount;
			const VkDebugUtilsObjectNameInfoEXT*         pObjects;
		} VkDebugUtilsMessengerCallbackDataEXT;
 *
 * @param pUserData optional data specified by the user
 * @return VkBool32
 */
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

//--------------------------------------------------------------------------------
vk::DebugUtilsMessengerEXT createDebugMessenger(vk::Instance& instance, vk::detail::DispatchLoaderDynamic& dldi)
{
	/**
	 * from vulkan_structs.hpp:
	 *
	 * VULKAN_HPP_CONSTEXPR DebugUtilsMessengerCreateInfoEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagsEXT flags_           = {},
															  VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagsEXT messageSeverity_ = {},
															  VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT     messageType_     = {},
															  PFN_vkDebugUtilsMessengerCallbackEXT                    pfnUserCallback_ = {},
															  void *                                                  pUserData_       = {},
															  const void *                                            pNext_ = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	const vk::DebugUtilsMessengerCreateInfoEXT createInfo(
		vk::DebugUtilsMessengerCreateFlagsEXT(),
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback,
		nullptr
	);
	return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}

//--------------------------------------------------------------------------------
vk::SurfaceKHR createSurfaceKHR(vk::Instance& instance, wnd::WindowManager* windowManager)
{
#if defined(CF__CURLY_EDITOR_OS_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	/**
	 * from vulkan_structs.hpp
	 *
	 * VULKAN_HPP_CONSTEXPR Win32SurfaceCreateInfoKHR( VULKAN_HPP_NAMESPACE::Win32SurfaceCreateFlagsKHR flags_     = {},
													   HINSTANCE                                        hinstance_ = {},
													   HWND                                             hwnd_      = {},
													   const void *                                     pNext_     = nullptr ) VULKAN_HPP_NOEXCEPT
	 */
	const vk::Win32SurfaceCreateInfoKHR createInfo(
		vk::Win32SurfaceCreateFlagsKHR(),
		wnd::WindowManager::getProcInstanceRawHandle(),
		windowManager->getRawHandle()
	);
	return instance.createWin32SurfaceKHR(createInfo);
#elif defined(CF__CURLY_EDITOR_OS_LINUX) && defined(VK_USE_PLATFORM_XLIB_KHR)
	// TODO: Linux Support
	const vk::XlibSurfaceCreateInfoKHR createInfo(
		vk::XlibSurfaceCreateFlagsKHR(),
		wnd::WindowManager::getRawDisplay(),
		windowManager->getRawHandle()
	);
	return instance.createXlibSurfaceKHR(createInfo);
#else
	return nullptr;
#endif
}

} // namespace vkUtils
