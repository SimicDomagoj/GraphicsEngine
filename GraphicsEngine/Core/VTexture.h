#pragma once

#include<vulkan\vulkan.hpp>

/**
	Vulkan texture class
	Class representing a Vulkan texture contained in the GPU.
*/
class VTexture
{
public:
	/**
		Constructor.
	*/
	VTexture();
	VTexture(VTexture& x) = delete;
	/**
		Move constructor.
	*/
	VTexture(VTexture&& x);
	VTexture& operator=(VTexture& x) = delete;
	/**
		Move assignment operator.
	*/
	VTexture& operator=(VTexture&& x);
	/**
		Returns a handle to a image view object.
		@return handle to a image view object.
	*/
	vk::ImageView getImageView() const;
	/**
		Returns a handle to a sampler object.
		@return handle of a sampler object.
	*/
	vk::Sampler getSampler() const;
	/**
		Returns width of a texture.
		@return texture's width.
	*/
	uint32_t getWidth() const;
	/**
		Returns height of a texture.
		@return texture's height.
	*/
	uint32_t getHeight() const;
	/**
		Destructor
	*/
	~VTexture();
	friend class VulkanBase;
private:
	/**
		Resets all variables.
	*/
	void clear();
	const vk::Device* device;				//*< Pointer to a logic device used to create a Vulkan texture.
	vk::Image textureImage;					//*< Texture's handle
	vk::DeviceMemory textureImageMemory;	//*< Handle of a device memory in which texture is stored.
	vk::ImageView textureImageView;			//*< Handle of a image view object which is used to acces image.
	vk::Sampler sampler;					//*< Handle of a sampler object.
	uint32_t texWidth;						//*< Texture's width.
	uint32_t texHeight;						//*< Texture's height.
};