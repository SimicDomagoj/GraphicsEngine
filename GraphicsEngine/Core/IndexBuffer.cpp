#include"IndexBuffer.h"

IndexBuffer::IndexBuffer(IndexBuffer && x) : StaticBuffer(std::move(x)), numIndices{ x.numIndices } {}

IndexBuffer & IndexBuffer::operator=(IndexBuffer&& x)
{
	StaticBuffer::operator=(std::move(x));
	numIndices = x.numIndices;
	x.numIndices = 0;
	return *this;
}

uint32_t IndexBuffer::getIndicesCount() const
{
	return numIndices;
}

void IndexBuffer::bind(const vk::CommandBuffer& buffer, const uint32_t bindingPoint) const
{
	buffer.bindIndexBuffer(this->buffer, 0, vk::IndexType::eUint32);
}