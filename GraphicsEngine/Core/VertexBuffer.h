#pragma once
#include"StaticBuffer.h"

/**
	Index buffer class.
	Class used to store vertices in GPU.
*/
class VertexBuffer : public StaticBuffer
{
public:
	/**
		Constructor.
	*/
	VertexBuffer() {}
	VertexBuffer(VertexBuffer& x) = delete;
	/**
		Move constructor.
	*/
	VertexBuffer(VertexBuffer&& x) : StaticBuffer(std::move(x)) {}
	VertexBuffer& operator=(VertexBuffer& x) = delete;
	/**
		Move assignment operator.
	*/
	VertexBuffer& operator=(VertexBuffer&& x);
	/**
		Binds a buffer to a binding point using commmand buffer.
		@param buffer command buffer used to bind static buffer.
		@param bindingPoint binding point in the shader set to which the buffer will be bound.
	*/
	virtual void bind(const vk::CommandBuffer& buffer, const uint32_t bindingPoint) const override;
	/**
		Destructor.
	*/
	~VertexBuffer() {}
};