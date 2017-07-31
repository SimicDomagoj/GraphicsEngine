#pragma once
#include"vulkan\vulkan.hpp"
#include<iostream>
#include<vector>

/**
	debug namespace
	Contains function callbacks and prototypes used by Vulkan for debugging purposes.
*/
namespace debug
{
	/**
		Array of char elements.
		Contains names of Vulkan validation layers we want to enable. 
	*/
	const std::vector<const char*> validationLayers{ "VK_LAYER_LUNARG_standard_validation" };

	/**
		Function prototype used by Vulkan when an error is encountered.
	*/
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
		uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData)
	{
		std::cerr << "validation layer: " << msg << std::endl;

		return VK_FALSE;
	}

	/**
		Creates a debug callback.
		@param instance vulkan instance on which we want to create a callback.
		@param pCreateInfo pointer to create info structure containing information about debug callback.
		@param pAllocator custom allocator.
		@param pCallback pointer to a memory location in which callback will be created.
	*/
	VkResult CreateDebugReportCallbackEXT(const vk::Instance& instance, const vk::DebugReportCallbackCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugReportCallbackEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(static_cast<VkInstance>(instance), "vkCreateDebugReportCallbackEXT");
		if (func != nullptr)
		{
			return func(static_cast<VkInstance>(instance), reinterpret_cast<const VkDebugReportCallbackCreateInfoEXT*>(pCreateInfo), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator), reinterpret_cast<VkDebugReportCallbackEXT*>(pCallback));
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	/**
		Destroys a debug callback.
		@param instance vulkan instace for which the callback was created.
		@param callback callback to be destroyed.
		@param pAllocator custom allocator.
	*/
	void DestroyDebugReportCallbackEXT(vk::Instance instance, vk::DebugReportCallbackEXT callback, const vk::AllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(static_cast<VkInstance>(instance), "vkDestroyDebugReportCallbackEXT");
		if (func != nullptr)
		{
			func(static_cast<VkInstance>(instance), static_cast<VkDebugReportCallbackEXT>(callback), reinterpret_cast<const VkAllocationCallbacks*>(pAllocator));
		}
	}

	void errorCallback(int, const char* err_str)
	{
		std::cout << "GLFW Error: " << err_str << std::endl;
	}
}