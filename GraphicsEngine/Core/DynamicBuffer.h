#pragma once
#include"vulkan\vulkan.hpp"

/**
	Dynamic buffer class
	Vulkan GPU buffer which can be updated.
*/
template <typename T>
class DynamicBuffer
{
public:
	/**
		Constructor.
	*/
	DynamicBuffer();
	DynamicBuffer(DynamicBuffer& x) = delete;
	/**
		Move constructor.
	*/
	DynamicBuffer(DynamicBuffer&& x);
	DynamicBuffer& operator=(DynamicBuffer& x) = delete;
	/**
		Move assignment operator.
	*/
	DynamicBuffer& operator=(DynamicBuffer&& x);
	/**
		Updates the buffer.
		@param newData data to write in the buffer.
	*/
	void updateBuffer(const T& newData);
	/**
		Cast to a Vulkan buffer handle.
		@return buffer's handle
	*/
	operator vk::Buffer() const;
	/**
		Returns a size of the buffer.
		@return buffer's size.
	*/
	std::size_t size() const;
	/**
		Destructor.
	*/
	~DynamicBuffer();
	friend class VulkanBase;
private:
	/**
		Resets all variables.
	*/
	void clear();
	const vk::Device* device;						//*< Pointer to a logic device used to create a buffer.
	const vk::CommandPool* pool;					//*< Pointer to a command pool.
	const vk::Queue* queue;							//*< Pointer to a gpu queue.
	vk::Buffer uniformStagingBuffer;				//*< Handle of a buffer used for staging.
	vk::DeviceMemory uniformStagingBufferMemory;	//*< Handle of a memory used for staging.
	vk::Buffer uniformBuffer;						//*< Buffer's handle.
	vk::DeviceMemory uniformBufferMemory;			//*< Buffer memory's handle.
};

template<typename T>
inline DynamicBuffer<T>::DynamicBuffer()
{
}

template<typename T>
inline DynamicBuffer<T>::DynamicBuffer(DynamicBuffer && x)
{
	device = x.device;
	pool = x.pool;
	queue = x.queue;
	uniformStagingBuffer = x.uniformStagingBuffer;
	uniformStagingBufferMemory = x.uniformStagingBufferMemory;
	uniformBuffer = x.uniformBuffer;
	uniformBufferMemory = x.uniformBufferMemory;

	x.device = nullptr;
	x.pool = nullptr;
	x.queue = nullptr;
	x.uniformStagingBuffer = vk::Buffer();
	x.uniformStagingBufferMemory = vk::DeviceMemory();
	x.uniformBuffer = vk::Buffer();
	x.uniformBufferMemory = vk::DeviceMemory();
}

template<typename T>
inline DynamicBuffer<T>& DynamicBuffer<T>::operator=(DynamicBuffer && x)
{
	if (this != &x)
	{
		clear();
		device = x.device;
		pool = x.pool;
		queue = x.queue;
		uniformStagingBuffer = x.uniformStagingBuffer;
		uniformStagingBufferMemory = x.uniformStagingBufferMemory;
		uniformBuffer = x.uniformBuffer;
		uniformBufferMemory = x.uniformBufferMemory;

		x.device = nullptr;
		x.pool = nullptr;
		x.queue = nullptr;
		x.uniformStagingBuffer = vk::Buffer();
		x.uniformStagingBufferMemory = vk::DeviceMemory();
		x.uniformBuffer = vk::Buffer();
		x.uniformBufferMemory = vk::DeviceMemory();
	}
	return *this;
}

template<typename T>
inline void DynamicBuffer<T>::updateBuffer(const T& newData)
{
	//map gpu buffer to cpu memory.
	void* data = device->mapMemory(uniformStagingBufferMemory, 0, sizeof(newData));
	//copy data to buffer
	memcpy(data, &newData, sizeof(newData));
	//unmap memory
	device->unmapMemory(uniformStagingBufferMemory);

	//allocate command buffer
	vk::CommandBufferAllocateInfo allocInfo{ *pool, vk::CommandBufferLevel::ePrimary, 1 };
	vk::CommandBuffer commandBuffer = device->allocateCommandBuffers(allocInfo)[0];
	vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
	commandBuffer.begin(beginInfo);

	vk::BufferCopy region{ 0, 0, sizeof(newData) };
	//copy staging buffer to uniform buffer
	commandBuffer.copyBuffer(uniformStagingBuffer, uniformBuffer, region);

	commandBuffer.end();
	//execute command buffer
	vk::SubmitInfo submitInfo{ 0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr };
	queue->submit(submitInfo, vk::Fence());
	queue->waitIdle();
	device->freeCommandBuffers(*pool, commandBuffer);
}

template<typename T>
inline DynamicBuffer<T>::operator vk::Buffer() const
{
	return uniformBuffer;
}

template<typename T>
inline std::size_t DynamicBuffer<T>::size() const
{
	return sizeof(T);
}

template<typename T>
inline DynamicBuffer<T>::~DynamicBuffer()
{
	clear();
}

template<typename T>
inline void DynamicBuffer<T>::clear()
{
	if (uniformStagingBufferMemory)
	{
		device->freeMemory(uniformStagingBufferMemory);
	}
	if (uniformStagingBuffer)
	{
		device->destroyBuffer(uniformStagingBuffer);
	}
	if (uniformBufferMemory)
	{
		device->freeMemory(uniformBufferMemory);
	}
	if (uniformBuffer)
	{
		device->destroyBuffer(uniformBuffer);
	}
}
