#include "VulkanBase.h"
#include "..\DebugTools\Debug.h"
#include <glm/gtx/transform.hpp>
#include "SwapChainSupportDetails.h"
#include "VTexture.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"..\DebugTools\Assert.h"

void VulkanBase::init(const char * appName, const bool& validating, uint32_t screenWidth, uint32_t screenHeight)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	createInstance(appName, validating);
	if (validating)
	{
		setupDebugCallback();
	}
	createSurface(appName, screenWidth, screenHeight);
	pickPhysicalDevice();
	createLogicalDevice(validating);
	swapExtent = vk::Extent2D(screenWidth, screenHeight);
	createSwapChain();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createFramebuffers();
	createDescriptorPool();
	createSemaphores();
}

glm::vec2 VulkanBase::getScreenSize() const
{
	return glm::vec2(swapExtent.width, swapExtent.height);
}

void VulkanBase::setWindowSize(uint32_t width, uint32_t height)
{
	glfwSetWindowSize(window, width, height);
	swapExtent.width = width;
	swapExtent.height = height;
	recreateSwapChain();
}

VTexture VulkanBase::createTexture(unsigned char * pixels, unsigned int width, unsigned int height, bool useStaging) const
{
	ASSERT(pixels != nullptr)
	VTexture tex{};
	tex.device = &logicDevice;
	tex.texWidth = width;
	tex.texHeight = height;

	vk::DeviceSize imageSize = tex.texWidth * tex.texHeight * 4;
	
	vk::Image stagingImage;
	vk::DeviceMemory stagingImageMemory;
	stagingImage = createImage(vk::Extent3D{ static_cast<uint32_t>(tex.texWidth), static_cast<uint32_t>(tex.texHeight), 1 }, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eLinear, vk::ImageUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, &stagingImageMemory);

	vk::ImageSubresource subresource{ vk::ImageAspectFlagBits::eColor, 0, 0 };
	vk::SubresourceLayout stagingImageLayout = logicDevice.getImageSubresourceLayout(stagingImage, subresource);

	void* data = logicDevice.mapMemory(stagingImageMemory, 0, imageSize);
	if (stagingImageLayout.rowPitch == tex.texWidth * 4)
	{
		memcpy(data, pixels, (size_t)imageSize);
	}
	else
	{
		uint8_t* dataBytes = reinterpret_cast<uint8_t*>(data);

		for (uint32_t y = 0; y < tex.texHeight; y++)
		{
			memcpy(&dataBytes[y * stagingImageLayout.rowPitch], &pixels[y * tex.texWidth * 4], tex.texWidth * 4);
		}
	}
	logicDevice.unmapMemory(stagingImageMemory);

	tex.textureImage = createImage(vk::Extent3D{ static_cast<uint32_t>(tex.texWidth), static_cast<uint32_t>(tex.texHeight), 1 }, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, &tex.textureImageMemory);

	transitionImageLayout(stagingImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferSrcOptimal);
	transitionImageLayout(tex.textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::ePreinitialized, vk::ImageLayout::eTransferDstOptimal);
	copyImage(stagingImage, tex.textureImage, tex.texWidth, tex.texHeight);
	transitionImageLayout(tex.textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

	logicDevice.freeMemory(stagingImageMemory);
	logicDevice.destroyImage(stagingImage);

	tex.textureImageView = createImageView(tex.textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor);

	vk::SamplerCreateInfo samplerInfo{ vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat, 0.0f,
		VK_TRUE, 16, VK_FALSE, vk::CompareOp::eAlways, 0.0f, 0.0f, vk::BorderColor::eIntOpaqueBlack, VK_FALSE };

	tex.sampler = logicDevice.createSampler(samplerInfo);
	return tex;
}

GLFWwindow * VulkanBase::getWindow() const
{
	return window;
}

VulkanBase::~VulkanBase()
{
	logicDevice.destroySemaphore(renderFinishedSemaphore);
	logicDevice.destroySemaphore(imageAvailableSemaphore);
	logicDevice.destroyImageView(depthImageView);
	logicDevice.freeMemory(depthImageMemory);
	logicDevice.destroyImage(depthImage);
	logicDevice.destroyDescriptorPool(descriptorPool);
	logicDevice.destroyCommandPool(commandPool);
	for (auto& pipe : graphicsPipelines)
	{
		logicDevice.destroyPipeline(pipe.handle);
	}
	graphicsPipelines.clear();
	pipelineLayouts.clear();
	for (auto& layout : descriptorLayouts)
	{
		logicDevice.destroyDescriptorSetLayout(layout);
	}
	descriptorLayouts.clear();
	for (auto& buff : swapFramebuffers)
	{
		logicDevice.destroyFramebuffer(buff);
	}
	swapFramebuffers.clear();
	logicDevice.destroyRenderPass(renderPass);
	for (auto& view : swapImageViews)
	{
		logicDevice.destroyImageView(view);
	}
	swapImageViews.clear();
	logicDevice.destroySwapchainKHR(swapChain);
	logicDevice.destroy();
	instance.destroySurfaceKHR(surface);
	glfwDestroyWindow(window);
	debug::DestroyDebugReportCallbackEXT(instance, debug, nullptr);
	instance.destroy();
}

VertexBuffer VulkanBase::createVertexBuffer(void* vertices, const size_t& bufferSize, bool useStaging) const
{
	VertexBuffer buffer;
	buffer.device = &logicDevice;

	vk::DeviceSize size = bufferSize;
	if (useStaging)
	{
		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		stagingBuffer = createBuffer(size, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, &stagingBufferMemory);

		void* data = logicDevice.mapMemory(stagingBufferMemory, 0, size);
		memcpy(data, vertices, bufferSize);
		logicDevice.unmapMemory(stagingBufferMemory);

		buffer.buffer = createBuffer(size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, &buffer.memory);
		copyBuffer(stagingBuffer, buffer.buffer, size);

		logicDevice.freeMemory(stagingBufferMemory);
		logicDevice.destroyBuffer(stagingBuffer);
	}
	else
	{
		buffer.buffer = createBuffer(size, vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eHostVisible, &buffer.memory);
		void* data = logicDevice.mapMemory(buffer.memory, 0, size);
		memcpy(data, vertices, bufferSize);
		logicDevice.unmapMemory(buffer.memory);
	}
	return buffer;
}

IndexBuffer VulkanBase::createIndexBuffer(const std::vector<uint32_t>& indices, bool useStaging) const
{
	IndexBuffer buffer;
	buffer.device = &logicDevice;
	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	buffer.numIndices = indices.size();
	if (useStaging)
	{
		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		stagingBuffer = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, &stagingBufferMemory);

		void* data = logicDevice.mapMemory(stagingBufferMemory, 0, bufferSize);
		memcpy(data, indices.data(), (size_t)bufferSize);
		logicDevice.unmapMemory(stagingBufferMemory);

		buffer.buffer = createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, &buffer.memory);
		copyBuffer(stagingBuffer, buffer.buffer, bufferSize);

		logicDevice.freeMemory(stagingBufferMemory);
		logicDevice.destroyBuffer(stagingBuffer);
	}
	else
	{
		buffer.buffer = createBuffer(bufferSize, vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eHostVisible, &buffer.memory);
		void* data = logicDevice.mapMemory(buffer.memory, 0, bufferSize);
		memcpy(data, indices.data(), (size_t)bufferSize);
		logicDevice.unmapMemory(buffer.memory);
	}
	return buffer;
}

void VulkanBase::createInstance(const char * appName, const bool & validating)
{
	vk::ApplicationInfo appInfo{appName, VK_MAKE_VERSION(1, 0, 0),
									"DSEngine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0 };

	auto extensions = getRequiredExtensions(validating);

	vk::InstanceCreateInfo instInfo = { vk::InstanceCreateFlags(), &appInfo, 0, nullptr, extensions.size(), extensions.data() };
	if (validating)
	{
		instInfo.enabledLayerCount = debug::validationLayers.size();
		instInfo.ppEnabledLayerNames = debug::validationLayers.data();
	}
	instance = vk::createInstance(instInfo);
}

void VulkanBase::setupDebugCallback()
{
	vk::DebugReportCallbackCreateInfoEXT debugInfo{ vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning,
													debug::debugCallback, nullptr };
	debug::CreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debug);
	glfwSetErrorCallback(debug::errorCallback);
}

void VulkanBase::createSurface(const char * appName, uint32_t screenWidth, uint32_t screenHeight)
{
	window = glfwCreateWindow(screenWidth, screenHeight, appName, nullptr, nullptr);
	VkResult result = glfwCreateWindowSurface(static_cast<VkInstance>(instance), window, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface));
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void VulkanBase::pickPhysicalDevice()
{
	std::vector<vk::PhysicalDevice> devs = instance.enumeratePhysicalDevices();
	physDev = devs[0];
}

void VulkanBase::createLogicalDevice(const bool & validating)
{
	uint32_t queueIndex = findQueueIndex(physDev, vk::QueueFlagBits::eGraphics, surface);
	float queuePriority = 1.f;

	vk::DeviceQueueCreateInfo queueInfo{ vk::DeviceQueueCreateFlags(), queueIndex, 1, &queuePriority };
	vk::PhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.fillModeNonSolid = VK_TRUE;
	const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	vk::DeviceCreateInfo deviceInfo{ vk::DeviceCreateFlags(), 1, &queueInfo, 0, nullptr, deviceExtensions.size(), deviceExtensions.data(), &deviceFeatures };
	if (validating)
	{
		deviceInfo.enabledLayerCount = debug::validationLayers.size();
		deviceInfo.ppEnabledLayerNames = debug::validationLayers.data();
	}

	logicDevice = physDev.createDevice(deviceInfo);

	queue = logicDevice.getQueue(queueIndex, 0);
}

void VulkanBase::createSwapChain()
{
	ASSERT(swapExtent.width > 0 && swapExtent.height > 0)
	SwapChainSupportDetails detail = querySwapChainSupport(physDev, surface);

	vk::SurfaceFormatKHR format = chooseSwapSurfaceFormat(detail.formats);
	vk::PresentModeKHR mode = chooseSwapPresentMode(detail.presentModes);
	vk::Extent2D extent = chooseSwapExtent(detail.capabilities, swapExtent);

	uint32_t imageCnt = detail.capabilities.minImageCount + 1;
	if (detail.capabilities.maxImageCount > 0 && imageCnt > detail.capabilities.maxImageCount)
	{
		imageCnt = detail.capabilities.maxImageCount;
	}

	vk::SwapchainKHR oldSwapChain = swapChain;

	vk::SwapchainCreateInfoKHR swapInfo{ vk::SwapchainCreateFlagsKHR(), surface, imageCnt,
										format.format, format.colorSpace, extent,
										1, vk::ImageUsageFlagBits::eColorAttachment,
										vk::SharingMode::eExclusive, 0, nullptr,
										detail.capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque,
										mode, VK_TRUE, oldSwapChain };

	vk::SwapchainKHR newSwapChain;
	newSwapChain = logicDevice.createSwapchainKHR(swapInfo);
	swapChain = newSwapChain;
	if (oldSwapChain)
	{
		logicDevice.destroySwapchainKHR(oldSwapChain);
	}

	swapImages = logicDevice.getSwapchainImagesKHR(swapChain);

	swapFormat = format.format;
	swapExtent = extent;

	if (swapImageViews.size() > 0)
	{
		for (auto& view : swapImageViews)
		{
			logicDevice.destroyImageView(view);
		}
	}
	swapImageViews.resize(swapImages.size());

	for (uint32_t i = 0; i < swapImages.size(); i++)
	{
		swapImageViews[i] = createImageView(swapImages[i], swapFormat, vk::ImageAspectFlagBits::eColor);
	}
}

void VulkanBase::recreateSwapChain()
{
	logicDevice.waitIdle();

	createSwapChain();
	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
}

void VulkanBase::createFramebuffers()
{
	if (depthImageView)
	{
		logicDevice.destroyImageView(depthImageView);
	}
	if (depthImageMemory)
	{
		logicDevice.freeMemory(depthImageMemory);
	}
	if (depthImage)
	{
		logicDevice.destroyImage(depthImage);
	}

	vk::Format depthFormat = findDepthFormat();
	vk::Extent3D extent{ swapExtent.width, swapExtent.height, 1 };
	depthImage = createImage(extent, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, &depthImageMemory);
	depthImageView = createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);

	transitionImageLayout(depthImage, depthFormat, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	if (swapFramebuffers.size() > 0)
	{
		for (auto& buff : swapFramebuffers)
		{
			logicDevice.destroyFramebuffer(buff);
		}
	}
	swapFramebuffers.resize(swapImageViews.size());

	for (size_t i = 0; i < swapImageViews.size(); i++)
	{
		std::array<vk::ImageView, 2> attachments = {
			swapImageViews[i],
			depthImageView
		};

		vk::FramebufferCreateInfo framebufferInfo{ {}, renderPass, attachments.size(), attachments.data(),
												swapExtent.width, swapExtent.height, 1 };
		swapFramebuffers[i] = logicDevice.createFramebuffer(framebufferInfo);
	}
}

void VulkanBase::createCommandPool()
{
	vk::CommandPoolCreateInfo poolInfo{ {}, findQueueIndex(physDev, vk::QueueFlagBits::eGraphics, surface) };
	commandPool = logicDevice.createCommandPool(poolInfo);
}

void VulkanBase::createSemaphores()
{
	vk::SemaphoreCreateInfo semaphoreInfo{};
	imageAvailableSemaphore = logicDevice.createSemaphore(semaphoreInfo);
	renderFinishedSemaphore = logicDevice.createSemaphore(semaphoreInfo);
}

std::vector<const char*> VulkanBase::getRequiredExtensions(bool enableValidationLayers)
{
	unsigned int glfwExstensionCnt = 0;
	const char** glfwExtension;
	glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExstensionCnt);

	std::vector<const char*> extensions(glfwExstensionCnt + enableValidationLayers);

	for (unsigned int i = 0; i < glfwExstensionCnt; i++)
	{
		extensions[i] = glfwExtension[i];
	}

	if (enableValidationLayers)
	{
		extensions[glfwExstensionCnt] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
	}

	return extensions;
}

uint32_t VulkanBase::findQueueIndex(const vk::PhysicalDevice& device, const vk::QueueFlagBits& flags, const vk::SurfaceKHR& present)
{
	std::vector<vk::QueueFamilyProperties> queues = device.getQueueFamilyProperties();

	for (uint32_t i = 0; i < queues.size(); i++)
	{
		if ((queues[i].queueFlags & flags) && device.getSurfaceSupportKHR(i, present))
		{
			return i;
		}
	}
	throw std::runtime_error("no soutable queues found!");
}

SwapChainSupportDetails VulkanBase::querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
	SwapChainSupportDetails details;
	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}

vk::SurfaceFormatKHR VulkanBase::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
	{
		return{ vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}
	for (const vk::SurfaceFormatKHR& format : availableFormats)
	{
		if (format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear && format.format == vk::Format::eR8G8B8A8Unorm)
		{
			return format;
		}
	}
	return availableFormats[0];
}

vk::PresentModeKHR VulkanBase::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == vk::PresentModeKHR::eMailbox)
		{
			return availablePresentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D VulkanBase::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& desired)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	vk::Extent2D extent = desired;

	extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, extent.width));
	extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, extent.height));

	return extent;
}

vk::Format VulkanBase::findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for (vk::Format format : candidates)
	{
		vk::FormatProperties props = physDev.getFormatProperties(format);
		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format!");
}

uint32_t VulkanBase::findMemoryType(const vk::PhysicalDevice & device, uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
{
	vk::PhysicalDeviceMemoryProperties memProperties = device.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

vk::CommandBuffer VulkanBase::beginSingleTimeCommands() const
{
	vk::CommandBufferAllocateInfo allocInfo{ commandPool, vk::CommandBufferLevel::ePrimary, 1 };

	vk::CommandBuffer commandBuffer = logicDevice.allocateCommandBuffers(allocInfo)[0];

	vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eOneTimeSubmit };

	commandBuffer.begin(beginInfo);

	return commandBuffer;
}

void VulkanBase::endSingleTimeCommands(vk::CommandBuffer commandBuffer) const
{
	commandBuffer.end();

	vk::SubmitInfo submitInfo{ 0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr };

	queue.submit(submitInfo, vk::Fence());
	queue.waitIdle();
	logicDevice.freeCommandBuffers(commandPool, commandBuffer);
}

vk::Buffer VulkanBase::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::DeviceMemory * bufferMemory) const
{
	vk::Buffer buffer;
	vk::BufferCreateInfo bufferInfo{ vk::BufferCreateFlags(), size, usage, vk::SharingMode::eExclusive, 0, nullptr };

	buffer = logicDevice.createBuffer(bufferInfo);

	vk::MemoryRequirements memRequirements = logicDevice.getBufferMemoryRequirements(buffer);

	vk::MemoryAllocateInfo allocInfo{ memRequirements.size, findMemoryType(physDev, memRequirements.memoryTypeBits, properties) };

	*bufferMemory = logicDevice.allocateMemory(allocInfo);

	logicDevice.bindBufferMemory(buffer, *bufferMemory, 0);

	return buffer;
}

void VulkanBase::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) const
{
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::BufferCopy region = {0, 0, size};
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, region);

	endSingleTimeCommands(commandBuffer);
}

void VulkanBase::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::BufferCopy copyRegion) const
{
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();
	
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

	endSingleTimeCommands(commandBuffer);
}

vk::Format VulkanBase::findDepthFormat()
{
	return findSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}
//NOTE helper only. device needs to destroy it
vk::Image VulkanBase::createImage(vk::Extent3D extent, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::DeviceMemory * imageMemory) const
{
	vk::ImageCreateInfo imageInfo{ vk::ImageCreateFlags(), vk::ImageType::e2D, format, extent, 1,1,vk::SampleCountFlagBits::e1,
		tiling, usage, vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::ePreinitialized };

	vk::Image image = logicDevice.createImage(imageInfo);

	vk::MemoryRequirements memRequirements = logicDevice.getImageMemoryRequirements(image);

	vk::MemoryAllocateInfo allocInfo{ memRequirements.size, findMemoryType(physDev, memRequirements.memoryTypeBits, properties) };

	*imageMemory = logicDevice.allocateMemory(allocInfo);

	logicDevice.bindImageMemory(image, *imageMemory, 0);

	return image;
}

void VulkanBase::copyImage(vk::Image srcImage, vk::Image dstImage, uint32_t width, uint32_t height) const
{
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::ImageSubresourceLayers subResource{ vk::ImageAspectFlagBits::eColor, 0, 0 ,1 };

	vk::ImageCopy region{ subResource, vk::Offset3D{0,0,0}, subResource, vk::Offset3D{ 0,0,0 }, vk::Extent3D{width, height, 1} };

	commandBuffer.copyImage(srcImage, vk::ImageLayout::eTransferSrcOptimal, dstImage,vk::ImageLayout::eTransferDstOptimal, 1, &region);

	endSingleTimeCommands(commandBuffer);
}

void VulkanBase::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) const
{
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier{ {},{}, oldLayout, newLayout, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
		image,{} };

	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (hasStencilComponent(format))
		{
			barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	}
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferSrcOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
	}
	else if (oldLayout == vk::ImageLayout::ePreinitialized && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlags{};
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTopOfPipe, vk::DependencyFlags(), nullptr, nullptr, barrier);

	endSingleTimeCommands(commandBuffer);
}

bool VulkanBase::hasStencilComponent(vk::Format format) const
{
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

vk::ImageView VulkanBase::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const
{
	vk::ImageView imageView;
	vk::ImageViewCreateInfo viewInfo{ vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format,
		vk::ComponentMapping{ vk::ComponentSwizzle::eIdentity,vk::ComponentSwizzle::eIdentity ,vk::ComponentSwizzle::eIdentity ,vk::ComponentSwizzle::eIdentity },
		vk::ImageSubresourceRange{ aspectFlags, 0, 1, 0, 1 } };

	imageView = logicDevice.createImageView(viewInfo);

	return imageView;
}
