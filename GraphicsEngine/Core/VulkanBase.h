#pragma once
#define GLFW_INCLUDE_VULKAN
#include"GLFW\glfw3.h"
#include<glm\glm.hpp>
#include<vulkan\vulkan.hpp>
#include "DynamicBuffer.h"
#include"Pipeline.h"
#include"GraphicsEngine.h"

struct SwapChainSupportDetails;

/**
	Class implementing all methods and containing variables which all applications using Vulkan have in common.
*/
class VulkanBase : public GraphicsEngine
{
public:
	/**
		Initializes vulkan and all other neccessaty commponents.
		@param appName name of the application.
		@param validation flag determining if debugging features are turned on.
		@param screenWidth screen's width we want to set.
		@param screenHeight screen's height we want to set.
	*/
	virtual void init(const char* appName, const bool& validating, uint32_t screenWidth, uint32_t screenHeight);
	/**
		Returns the screen size.
		@return screen size as two dimensional vector. First value is width, and second is height.
	*/
	glm::vec2 getScreenSize() const override;
	/**
		Sets the size of the screen.
		@param width screen's new width.
		@param height screen's new height.
	*/
	void setWindowSize(uint32_t width, uint32_t height) override;
	/**
		Creates a vertex buffer. Buffer used by the GPU which stores vertices.
		@param data void pointer to an array of vertices in an arbitrary vertex format.
		@param bufferSize size of data contained in array pointed to by \p data pointer.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return VertexBuffer object created with given parameters.
	*/
	VertexBuffer createVertexBuffer(void* data, const size_t& bufferSize, bool useStaging = true) const override;
	/**
		Creates an index buffer. Buffer used by the GPU which stores indices.
		@param indices void pointer to a vector of indices.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return IndexBuffer object created with given parameters.
	*/
	IndexBuffer createIndexBuffer(const std::vector<uint32_t>& indices, bool useStaging = true) const override;
	/**
		Creates a texture used by GPU.
		@param pixels pointer to an array of pixels. Data needs to be in 4 channel format.
		@param width texture's width.
		@param height texture's height.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return VTexture object containing created texture stored in the GPU.
	*/
	VTexture createTexture(unsigned char* pixels, unsigned int width, unsigned int height, bool useStaging = true) const override;
	/**
		Creates a graphics components, for desired game object, which holds all neccessary information to draw an object.
		@param id id of the game object for which we create a component. Id will be assigned to a component as well so it has a connection to a game object.
		Uniqueness of the id needs to be handeled externally, otherwise problems may arrise if there happen to be components with same ids.
		@param model filename of the model used.
		@param texFilename filename of the texture used.
		@param pipeline enumerator which determines how we want to draw the object.
		@param layer layer to which the object is assigned.
		@param loadType type of the model we want to use.
		@return shared pointer to the component created with given parameters.
	*/
	GLFWwindow* getWindow() const override;
	/**
		Destructor.
	*/
	virtual ~VulkanBase();
protected:
	vk::Instance instance;									//*< Instance is a connection between application and Vulkan library.
	vk::DebugReportCallbackEXT debug;						//*< Handle to a function which is used as a debug callback.
	GLFWwindow* window{ nullptr };							//*< Pointer to a glfw window in which we draw.
	vk::SurfaceKHR surface;									//*< Surface on which we render our images.
	vk::PhysicalDevice physDev;								//*< Handle to a physical device used.
	vk::Device logicDevice;									//*< Handle to a logical device created out of physical device.
	vk::Queue queue;										//*< Queue to which the command will be submitted for execution.
	vk::SwapchainKHR swapChain{};							//*< Handle to a swapchain which is basically queue out of which we get images waiting to be presented on screen. 
	vk::Format swapFormat;									//*< Swapchain's format.
	vk::Extent2D swapExtent;								//*< Swapchain's extent. Basically width and height of the screen.
	std::vector<vk::Image> swapImages;						//*< Array of images used by the swapchain to present the image to the screen.
	std::vector<vk::ImageView> swapImageViews;				//*< Array of image views used to access swapchain images.
	vk::RenderPass renderPass;								//*< Handle for the render pass which contains the information about framebuffer attachments and how to handle them.
	std::vector<vk::Framebuffer> swapFramebuffers;			//*< Array of framebuffers. Framebuffer is a collection of images used for a rendering operation.
	std::vector<vk::DescriptorSetLayout> descriptorLayouts;	//*< Array of layouts used to create descriptor sets.
	std::vector<PipelineLayout> pipelineLayouts;			//*< Array of layouts used to create pipelines.
	std::vector<Pipeline> graphicsPipelines;				//*< Array of graphics pipeline used to draw objects.
	vk::CommandPool commandPool;							//*< Handle to a pool used to allocate command buffers.
	vk::DescriptorPool descriptorPool;						//*< Handle to a pool used to allocate descriptor sets.
	vk::Image depthImage;									//*< Handle to an image used for representing depth.
	vk::DeviceMemory depthImageMemory;						//*< Handle to a memory used to store a depth image.
	vk::ImageView depthImageView;							//*< Handel to a image view used to access depth image.
	vk::Semaphore imageAvailableSemaphore;					//*< Semaphore used to signal when an image is avaliable so we can render to it.
	vk::Semaphore renderFinishedSemaphore;					//*< Semapjore used to signal that rendering is finished.

	/**
		Creates a buffer which is updated often.
		@return buffer used to store data in the GPU and is made so frequent updates are faster then static buffers.
	*/
	template <typename T>
	DynamicBuffer<T> createDynamicBuffer();
	/**
		Creates a instace of the Vulkan library.
		@param appName name of the application we are developing.
		@param validating flag determining if debug options are turned on or off.
	*/
	void createInstance(const char* appName, const bool& validating);
	/**
		Sets up a callback function used for debuging.
	*/
	void setupDebugCallback();
	/**
		Creates a surface in which we render our images.
		@param appName name of the application we are developing.
		@param screenWidth width of the surface.
		@param screenHeight height of the surface.
	*/
	void createSurface(const char* appName, uint32_t screenWidth, uint32_t screenHeight);
	/**
		Picks a physical device which will be used for rendering and other operation.
	*/
	void pickPhysicalDevice();
	/**
		Creates a logical device out of the selected physical device(s).
	*/
	void createLogicalDevice(const bool& validating);
	/**
		Creates a swapchain which is a queue out of which we get images waiting to be presented on screen.
	*/
	void createSwapChain();
	/**
		Recreates swapchain and all other neccessary components which need to be recreated when swapchain is recreated.
	*/
	virtual void recreateSwapChain();
	/**
		Creates a render pass. Render pass contains the information about framebuffer attachments.
		For example how many color and depth buffer are attached to it and how many samples are use for each of them.
		Also is holds the information how the contents of those buffers should be handled throughout the rendering operations.
	*/
	virtual void createRenderPass() = 0;
	/**
		Create framebuffers.
		Framebuffer is a collection of images or attachments created in a render pass, used for the rendering operation.
	*/
	void createFramebuffers();
	/**
		Creates descriptor set layout. Descriptor layout specifies the types of resources that a certain set in the shader has.
		For example it describes how many uniform variables, samplers or other variables a shader set has.
	*/
	virtual void createDescriptorSetLayout() = 0;
	/**
		Creates graphic pipelines used by the engine.
	*/
	virtual void createGraphicsPipeline() = 0;
	/**
	C	reates a pool out of which command buffers are allocated.
	*/
	void createCommandPool();
	/**
		Creates a pool out of which descriptors are allocated.
	*/
	virtual void createDescriptorPool() = 0;
	/**
		Creates semaphores neccessary for signalization between rendering phases.
	*/
	void createSemaphores();

	/**
		Finds desired memory type of a device.
		@param device physical device whose memory we want to get.
		@param typeFilter type of memory we want to get.
		@param properties properties desired memory needs to have.
		@return index of the found memory type.
	*/
	uint32_t findMemoryType(const vk::PhysicalDevice& device, uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
	/**
		Creates a command buffer and starts commands that will be used only once.
		@return command buffer to which we submit one time commands.
	*/
	vk::CommandBuffer beginSingleTimeCommands() const;
	/**
		Executes single use commands and destroys a buffer used to execute them.
		@param commandBuffer buffer to which we submited command and which we want to execute and finish.
	*/
	void endSingleTimeCommands(vk::CommandBuffer commandBuffer) const;
	/**
		Creates a Vulkan buffer.
		@param size size of the buffer.
		@param usage flags determining for what the buffer will be used.
		@param properties properties of a memory in which the buffer will be stored.
		@param[out] bufferMemory pointer to a memory handle in which memory associated with the buffer will be stored.
		@return created buffer.
	*/
	vk::Buffer createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::DeviceMemory* bufferMemory) const;
	/**
		Copies a buffer.
		@param srcBuffer buffer we want to copy.
		@param dstBuffer buffer in which we want to place the copied information.
		@param size size of part of the source buffer we want to copy.
	*/
	void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) const;
	/**
		Copies a buffer.
		@param srcBuffer buffer we want to copy.
		@param dstBuffer buffer in which we want to place the copied information.
		@param copyRegion region of the buffer we want to copy.
	*/
	void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::BufferCopy copyRegion) const;
	/**
		Finds which format has a depth component.
		@param return format with depth component.
	*/
	vk::Format findDepthFormat();
	/**
		Creates a Vulkan image.
		@param extent size of the image.
		@param format image's format.
		@param tiling tiling option for the image.
		@param usage flags determining for what operations the image will be used.
		@param properties properties of a memory in which the image will be stored.
		@param[out] imageMemory pointer to a memory handle in which memory associated with the image will be stored.
		@return created image.
	*/
	vk::Image createImage(vk::Extent3D extent, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::DeviceMemory* imageMemory) const;
	/**
		Copies an image.
		@param srcImage image we want to copy.
		@param dstImage image in which we want to place the copied information.
		@param width width of the copied image.
		@param height height of the copied image.
	*/
	void copyImage(vk::Image srcImage, vk::Image dstImage, uint32_t width, uint32_t height) const;
	/**
		Changes the layout of the desired image.
		@param image image whose layer we want to change.
		@param format image's format.
		@param oldLayout image's current layout.
		@param newLayout layout we want to set for the image.
	*/
	void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) const;
	/**
		Checks if given format has a stencil component.
		@param format handle of the format for which we want to check if it has a stencil component.
		@return true if format has stencil component, false otherwise.
	*/
	bool hasStencilComponent(vk::Format format) const;
	/**
		Creates an image view.
		@param image image for which te image view is created.
		@param format image's format.
		@param aspectFlags flags determining which aspects of the image are included in the view.
	*/
	vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) const;

private:
	/**
		Gets extension that are rewuired for application to work properly.
		@param enableValidationLayers flags that determines wheter or not the validation is enabled.
		@return requred extension.
	*/
	std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);
	/**
		Finds a queue index with neccessary features.
		@param device device on which we search for the queue index.
		@param flags flags determining neccessary queue features.
		@param present surface on which we want to draw.
		@return found queue index.
	*/
	uint32_t findQueueIndex(const vk::PhysicalDevice& device, const vk::QueueFlagBits& flags, const vk::SurfaceKHR& present);
	/**
		Finds the supported swapchain features.
		@param device device for which we want to create a swapchain.
		@param surface surface to which the swapchain will be attached.
		@return structure with supported swapchain features.
	*/
	SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
	/**
		Chooses a best suitable format for the surface.
		@param availableFormats formats out of which to pick.
		@return chosen format.
	*/
	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
	/**
		Chooses a best suitable presentation mode for the surface.
		@param availablePresentModes presentation modes out of which to pick.
		@return chosen presentation mode.
	*/
	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes);
	/**
		Chooses an appropriate swap extent for the surface.
		@param capabilities capabilities of a surface for which we are choosing.
		@param desired swap extent which would be prefered.
		@return chosen swap extent.
	*/
	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, const vk::Extent2D& desired);
	/**
		Finds a format with neccessaty features.
		@param candidates formats out of which the result will be chosen.
		@param tiling tiling feature which the format needs to have.
		@param features features which format need to have.
		@return chosen format.
	*/
	vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
};

template<typename T>
inline DynamicBuffer<T> VulkanBase::createDynamicBuffer()
{
	DynamicBuffer<T> buff;
	buff.device = &logicDevice;
	buff.pool = &commandPool;
	buff.queue = &queue;
	vk::BufferCreateInfo bufferInfo{ vk::BufferCreateFlags(), sizeof(T), vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, 0, nullptr };

	buff.uniformStagingBuffer = logicDevice.createBuffer(bufferInfo);

	vk::MemoryRequirements memRequirements = logicDevice.getBufferMemoryRequirements(buff.uniformStagingBuffer);

	vk::MemoryAllocateInfo allocInfo{ memRequirements.size, findMemoryType(physDev, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) };

	buff.uniformStagingBufferMemory = logicDevice.allocateMemory(allocInfo);
	logicDevice.bindBufferMemory(buff.uniformStagingBuffer, buff.uniformStagingBufferMemory, 0);
	//
	bufferInfo = vk::BufferCreateInfo{ vk::BufferCreateFlags(), sizeof(T), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer, vk::SharingMode::eExclusive, 0, nullptr };

	buff.uniformBuffer = logicDevice.createBuffer(bufferInfo);

	memRequirements = logicDevice.getBufferMemoryRequirements(buff.uniformBuffer);

	allocInfo = vk::MemoryAllocateInfo{ memRequirements.size, findMemoryType(physDev, memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal) };

	buff.uniformBufferMemory = logicDevice.allocateMemory(allocInfo);
	logicDevice.bindBufferMemory(buff.uniformBuffer, buff.uniformBufferMemory, 0);
	return buff;
}
