#include "DescriptorSet.h"

DescriptorSet::DescriptorSet() : set{ vk::DescriptorSet() }, layout{ nullptr }, use{ ShaderUsage::Empty }, device{ nullptr }, pool{ nullptr } {}

DescriptorSet::DescriptorSet(vk::DescriptorSet set, vk::DescriptorSetLayout * layout, const ShaderUsage & usage) :
	set{ set }, layout{ layout }, use{ usage }, device{ nullptr }, pool{ nullptr }{ }

DescriptorSet::DescriptorSet(DescriptorSet && x) :
	set{ x.set }, layout{ x.layout }, use{ x.use }, device{ x.device }, pool{ x.pool }
{
	x.set = vk::DescriptorSet();
	x.layout = nullptr;
	x.use = ShaderUsage::Empty;
	x.device = nullptr;
	x.pool = nullptr;
}

DescriptorSet & DescriptorSet::operator=(DescriptorSet && x)
{
	if (this != &x)
	{
		clear();
		set = x.set;
		layout = x.layout;
		use = x.use;
		device = x.device;
		pool = x.pool;

		x.set = vk::DescriptorSet();
		x.layout = nullptr;
		x.use = ShaderUsage::Empty;
		x.device = nullptr;
		x.pool = nullptr;
	}
	return *this;
}

DescriptorSet::operator vk::DescriptorSet() const
{
	return set;
}

void DescriptorSet::setDestructor(const vk::Device * device, const vk::DescriptorPool * pool)
{
	this->device = device;
	this->pool = pool;
}

vk::DescriptorSetLayout DescriptorSet::getDescriptorLayout() const
{
	return *layout;
}

ShaderUsage DescriptorSet::getUsage() const
{
	return use;
}

DescriptorSet::~DescriptorSet()
{
	clear();
}

void DescriptorSet::clear()
{
	if (set && device != nullptr && pool != nullptr)
	{
		device->freeDescriptorSets(*pool, set);
	}
	layout = nullptr;
	use = ShaderUsage::Empty;
	device = nullptr;
	pool = nullptr;
}

