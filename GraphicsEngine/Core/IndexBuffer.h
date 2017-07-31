#pragma once
#include"StaticBuffer.h"

/**
	Index buffer class.
	Class used to store indices in the GPU.
*/
class IndexBuffer : public StaticBuffer
{
public:
	/**
		Constructor.
	*/
	IndexBuffer() {}
	IndexBuffer(IndexBuffer& x) = delete;
	/**
		Move constructor.
	*/
	IndexBuffer(IndexBuffer&& x);
	IndexBuffer& operator=(IndexBuffer& x) = delete;
	/**
		Move assignment operator.
	*/
	IndexBuffer& operator=(IndexBuffer&& x);
	/**
		Returns number of indices stored in the buffer.
		@return  number of indices.
	*/
	uint32_t getIndicesCount() const;
	/**
		Binds a buffer to a binding point using commmand buffer.
		@param buffer command buffer used to bind static buffer.
		@param bindingPoint binding point in the shader set to which the buffer will be bound.
	*/
	virtual void bind(const vk::CommandBuffer& buffer, const uint32_t bindingPoint) const override;
	/**
		Destructor.
	*/
	~IndexBuffer() {}
	friend class VulkanBase;
protected:
	uint32_t numIndices;	//*< Number of indices stored in the buffer.
};