#pragma once

#include<vulkan\vulkan.hpp>
#include"ShaderUsage.h"

/**
	Stracture used to store pointers to two descriptors sets.
	One is a set that corresponds to shader's global set, and the other corresponds to a local set.
*/
struct DescriptorLayouts
{
	vk::DescriptorSetLayout* globalSet;	//*< Pointer to a set which corresponds to shader's global set.
	vk::DescriptorSetLayout* localSet;	//*< Pointer to a set which corresponds to shader's local set.
};

/**
	Pipeline layout class
	Class representing layout out of which a pipeline can be created.
*/
class PipelineLayout
{
public:
	/**
		Cosnstructor.
	*/
	PipelineLayout();
	/**
		Constructor.
		@param layout handle to a vulkan's pipeline layout handle.
		@param global pointer to a descriptor set layout of global shader variables used to create layout.
		@param local pointer to a descriptor set layout of local shader variables used to create layout.
	*/
	PipelineLayout(vk::Device* device, vk::PipelineLayout layout, vk::DescriptorSetLayout* global, vk::DescriptorSetLayout* local);
	PipelineLayout(const PipelineLayout& x) = delete;
	/**
		Move constructor.
	*/
	PipelineLayout(PipelineLayout&& x);
	PipelineLayout& operator=(const PipelineLayout& x) = delete;
	/**
		Move assignment operator;
	*/
	PipelineLayout& operator=(PipelineLayout&& x);
	/**
		Cast to vulkan's layout handle.
		@return vulkan's layout handle.
	*/
	operator vk::PipelineLayout() const;
	/**
		Returns a descriptor set layout of global shader variables used to create layout.
		@return descriptor set layout of global shader variables used to create layout.
	*/
	vk::DescriptorSetLayout* getGlobalSet() const;
	/**
		Returns a descriptor set layout of local shader variables used to create layout.
		@return descriptor set layout of local shader variables used to create layout.
	*/
	vk::DescriptorSetLayout* getLocalSet() const;
	/**
		Destructor.
	*/
	~PipelineLayout();
private:
	/**
		Resets all members.
	*/
	void clear();
	const vk::Device* device;			//*< Vulkan's logic device handle used by the layout.
	vk::PipelineLayout handle;			//*< Vulkan's layout handle.
	vk::DescriptorSetLayout* globalSet;	//*< pointer to a descriptor set layout of global shader variables used to create layout.
	vk::DescriptorSetLayout* localSet;	//*< pointer to a descriptor set layout of local shader variables used to create layout.
};

struct Pipeline
{
	vk::Pipeline handle;				//*< Vulkan's pipeline handle.
	PipelineLayout* layout;				//*< Pointer to a layout used to create the pipeline.
	ShaderUsage globalReq;				//*< Flags representing global shader variables needed for pipeline to function.
	ShaderUsage localReq;				//*< Flags representing local shader variables needed for pipeline to function.
};

