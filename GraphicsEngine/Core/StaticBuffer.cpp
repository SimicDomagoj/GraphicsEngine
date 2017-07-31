#include "StaticBuffer.h"

StaticBuffer::StaticBuffer(StaticBuffer&& x) : device{ x.device }, memory{ x.memory }, buffer{ x.buffer }
{
	x.device = nullptr;
	x.memory = vk::DeviceMemory();
	x.buffer = vk::Buffer();
}

StaticBuffer & StaticBuffer::operator=(StaticBuffer&& x)
{
	if (this != &x)
	{
		clear();
		device = x.device;
		memory = x.memory;
		buffer = x.buffer;

		x.device = nullptr;
		x.memory = vk::DeviceMemory();
		x.buffer = vk::Buffer();
	}
	return *this;
}

StaticBuffer::operator vk::Buffer() const
{
	return buffer;
}

StaticBuffer::~StaticBuffer()
{
	clear();
}

void StaticBuffer::clear()
{
	if (memory)
	{
		device->freeMemory(memory);
	}
	if (buffer)
	{
		device->destroyBuffer(buffer);
	}
	device = nullptr;
}