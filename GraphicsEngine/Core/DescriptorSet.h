#pragma once
#include"vulkan\vulkan.hpp"
#include"ShaderUsage.h"

/**
	Descriptor set
	Class representing descriptor set which describes a set used in shaders.
*/
class DescriptorSet
{
public:
	/**
		Constructor.
	*/
	DescriptorSet();
	/**
		Constructor.
		@param set handle of the descriptor set.
		@param layout pointer to the descriptor layout out of which the set was created.
		@param pipeLayout pointer to pipeline layout.
		@param usage flags representing shader variables represented by the set.
	*/
	DescriptorSet(vk::DescriptorSet set, vk::DescriptorSetLayout* layout, const ShaderUsage& usage);
	DescriptorSet(const DescriptorSet& x) = delete;
	/**
		Move constructor.
	*/
	DescriptorSet(DescriptorSet&& x);
	DescriptorSet& operator=(const DescriptorSet& x) = delete;
	/**
		Move assignment operator.
	*/
	DescriptorSet& operator=(DescriptorSet&& x);
	/**
		Cast operator
		Returns a vulkan descriptor set handle for the descriptor set.
	*/
	operator vk::DescriptorSet() const;
	/**
		Sets the variables needed for destruction of the descriptor.
		@param device logical device which was used to create the desriptor.
		@param pool pool from which the descriptor was allocated.
	*/
	void setDestructor(const vk::Device* device, const vk::DescriptorPool* pool);
	/**
		Returns a handle to the layout out of which the set was created.
		@return handle to the descriptor set layout out of which the set was created.
	*/
	vk::DescriptorSetLayout getDescriptorLayout() const;
	/**
		Returns shader set described by descriptor set.
		@return flags representing variables in a shader set described by descriptor. 
	*/
	ShaderUsage getUsage() const;
	/**
		Destructor.
	*/
	~DescriptorSet();
private:
	/**
		Resets all members.
	*/
	void clear();
	vk::DescriptorSet set;				//*< Descriptors vulkan handle.
	vk::DescriptorSetLayout* layout;	//*< Layout out of which the set was created.
	ShaderUsage use;					//*< Flags representing variables in shader set described by descriptor.
	const vk::Device* device;			//*< Pointer to a device used to create descriptor set.
	const vk::DescriptorPool* pool;		//*< Pointer to a pool out of which descriptor set was allocated.
};