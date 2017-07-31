#include"VertexBuffer.h"

VertexBuffer & VertexBuffer::operator=(VertexBuffer&& x)
{
	StaticBuffer::operator=(std::move(x));
	return *this;
}

void VertexBuffer::bind(const vk::CommandBuffer & buffer, const uint32_t bindingPoint) const
{
	std::vector<vk::DeviceSize>offsets{ vk::DeviceSize(0) };
	buffer.bindVertexBuffers(bindingPoint, this->buffer, offsets);
}