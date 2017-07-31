#pragma once
#include<vulkan\vulkan.hpp>
#include<utility>

/**
	Base class for static buffers.
	Class representing GPU's buffers that don't change.
*/
class StaticBuffer
{
public:
	/**
		Constructor.
	*/
	StaticBuffer() {}
	StaticBuffer(StaticBuffer& x) = delete;
	/**
		Move constructor.
	*/
	StaticBuffer(StaticBuffer&& x);
	StaticBuffer& operator=(StaticBuffer& x) = delete;
	/**
		Move assignment operator.
	*/
	StaticBuffer& operator=(StaticBuffer&& x);
	/**
		Casts buffer to a vulkan's buffer handle.
		@return vulkan's handle to a buffer.
	*/
	operator vk::Buffer() const;
	/**
		Binds a buffer to a binding point using commmand buffer.
		@param buffer command buffer used to bind static buffer.
		@param bindingPoint binding point in the shader set to which the buffer will be bound.
	*/
	virtual void bind(const vk::CommandBuffer& buffer, const uint32_t bindingPoint) const = 0;
	/**
		Destructor.
	*/
	virtual ~StaticBuffer();
	friend class VulkanBase;
protected:
	/**
		Resets all variables.
	*/
	void clear();
	const vk::Device* device;	//*< Pointer to a logic device used to create the buffer.
	vk::DeviceMemory memory;	//*< Vulkan's handle to a memory used by buffer.
	vk::Buffer buffer;			//*< Vulkan's buffer handle.
};