#pragma once
#include <vulkan\vulkan.hpp>
#include "ShaderUsage.h"

/**
	Shader class.
	Class that represents a shader.
*/
class Shader
{
public:
	/**
		Constructor.
	*/
	Shader();
	/** 
		Constructor.
		@param device logical device used to create a shader.
		@param filename filename of a file containing shader code.
		@param shaderType type of a sheder.
		@param globalUse global variables shader contains.
		@param localUse local variables shader contains.
	*/
	Shader(const vk::Device* device, const char* filename, const vk::ShaderStageFlagBits& shaderType, const ShaderUsage& globalUse, const ShaderUsage& localUse);
	Shader(Shader& x) = delete;
	/**
		Move constructor.
	*/
	Shader(Shader&& x);
	Shader& operator=(Shader& x) = delete;
	/**
		Move assignment operator.
	*/
	Shader& operator=(Shader&& x);
	/**
		Function used to create a shader. By calling this function existing shader will be replaced, if there is one.
		@param device logical device used to create a shader.
		@param filename filename of a file containing shader code.
		@param shaderType type of a sheder.
		@param globalUse global variables shader contains.
		@param localUse local variables shader contains.
	*/
	void createShader(const vk::Device* device, const char* filename, const vk::ShaderStageFlagBits& shaderType, const ShaderUsage& globalUse, const ShaderUsage& localUse);
	/**
		Returns information for creating a pipeline shader stage with this shader.
		@return information for creating a pipeline shader stage.
	*/
	vk::PipelineShaderStageCreateInfo getCreateInfo() const;
	/**
		Returns global variables which shader contains.
		@return flags representing global variables in the shader.
	*/
	ShaderUsage getGlobalUsage() const;
	/**
		Returns local variables which shader contains.
		@return flags representing local variables in the shader.
	*/
	ShaderUsage getLocalUsage() const;
	/**
		Destructor.
	*/
	~Shader();
private:
	/**
		Resets all variables.
	*/
	void clear();
	vk::ShaderModule module;				//*< Handle of a created shader.
	const vk::Device* device;						//*< Pointer to a logical device.
	vk::PipelineShaderStageCreateInfo info;	//*< Information for creating a pipeline shader stage.
	ShaderUsage global;						//*< Flags representing global variables in the shader.
	ShaderUsage local;						//*< Flags representing local variables in the shader.
};