#include "Shader.h"
#include <fstream>

static std::vector<char> readFile(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

Shader::Shader() : module{ }, device{ nullptr }, info{ } {}

Shader::Shader(const vk::Device * device, const char * filename, const vk::ShaderStageFlagBits & shaderType, const ShaderUsage& globalUse, const ShaderUsage& localUse)
{
	createShader(device, filename, shaderType, globalUse, localUse);
}

Shader::Shader(Shader && x)
{
	module = x.module;
	device = x.device;
	info = x.info;
	local = x.local;
	global = x.global;

	x.module = vk::ShaderModule();
	x.device = nullptr;
	x.info = {};
	x.local = ShaderUsage::Empty;
	x.global = ShaderUsage::Empty;
}

Shader & Shader::operator=(Shader && x)
{
	if (this != &x)
	{
		clear();
		module = x.module;
		device = x.device;
		info = x.info;
		local = x.local;
		global = x.global;

		x.module = vk::ShaderModule();
		x.device = nullptr;
		x.info = {};
		x.local = ShaderUsage::Empty;
		x.global = ShaderUsage::Empty;
	}
	return *this;
}

void Shader::createShader(const vk::Device * device, const char * filename, const vk::ShaderStageFlagBits & shaderType, const ShaderUsage& globalUse, const ShaderUsage& localUse)
{
	clear();

	global = globalUse;
	local = localUse;
	this->device = device;
	//read the file with the code
	const std::vector<char> code = readFile(filename);
	//fill the shader information.
	vk::ShaderModuleCreateInfo shaderInfo{ vk::ShaderModuleCreateFlags(), code.size(), (uint32_t*)code.data() };
	//create the shader.
	module = device->createShaderModule(shaderInfo);
	// fill pipeline shader stage creation info.
	info = vk::PipelineShaderStageCreateInfo{ vk::PipelineShaderStageCreateFlags(), shaderType, module, "main" };
}

vk::PipelineShaderStageCreateInfo Shader::getCreateInfo() const
{
	return info;
}

ShaderUsage Shader::getGlobalUsage() const
{
	return global;
}

ShaderUsage Shader::getLocalUsage() const
{
	return local;
}


Shader::~Shader()
{
	clear();
}

void Shader::clear()
{
	if (module)
	{
		device->destroyShaderModule(module);
		device = nullptr;
		info = {};
		global = ShaderUsage::Empty;
		local = ShaderUsage::Empty;
	}
}
