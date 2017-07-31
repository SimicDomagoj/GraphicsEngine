#pragma once
#include<vulkan\vulkan.hpp>

/**
	Swapchain support details structure
	Structure which contains details about swapchain.
*/
struct SwapChainSupportDetails
{
	vk::SurfaceCapabilitiesKHR capabilities;		//*< Structure containing surface capabilities.
	std::vector<vk::SurfaceFormatKHR> formats;		//*< Array of structures describing supported swapchain format-color space pairs.
	std::vector<vk::PresentModeKHR> presentModes;	//*< Array of enumeratos describing presentation modes.
};

