#include "Pipeline.h"
#include "..\DebugTools\Assert.h"

PipelineLayout::PipelineLayout() : device{ nullptr }, handle { vk::PipelineLayout() }, globalSet{ nullptr }, localSet{ nullptr } {}

PipelineLayout::PipelineLayout(vk::Device* device, vk::PipelineLayout layout, vk::DescriptorSetLayout* global, vk::DescriptorSetLayout* local) : device{ device }, handle { layout }, globalSet{ global }, localSet{ local } {}

PipelineLayout::PipelineLayout(PipelineLayout && x) : device{ x.device }, handle { x.handle }, globalSet{ x.globalSet }, localSet{ x.localSet }
{
	x.handle = vk::PipelineLayout();
	x.device = nullptr;
	x.globalSet = nullptr;
	x.localSet = nullptr;
}


PipelineLayout& PipelineLayout::operator=(PipelineLayout && x)
{
	if (this != &x)
	{
		handle = x.handle;
		device = x.device;
		globalSet = x.globalSet;
		localSet = x.localSet;

		x.handle = vk::PipelineLayout();
		x.device = nullptr;
		x.globalSet = nullptr;
		x.localSet = nullptr;
	}
	return *this;
}

PipelineLayout::operator vk::PipelineLayout() const
{
	return handle;
}

vk::DescriptorSetLayout* PipelineLayout::getGlobalSet() const
{
	return globalSet;
}

vk::DescriptorSetLayout* PipelineLayout::getLocalSet() const
{
	return localSet;
}

PipelineLayout::~PipelineLayout()
{
	clear();
}

void PipelineLayout::clear()
{
	if (device != nullptr)
	{
		ASSERT(handle);
		device->destroyPipelineLayout(handle);
	}
	handle = vk::PipelineLayout();
	device = nullptr;
	globalSet = nullptr;
	localSet = nullptr;
}

