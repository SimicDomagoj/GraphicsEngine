#include "VTexture.h"

VTexture::VTexture() : device{ nullptr }, textureImage{ vk::Image() }, textureImageMemory{ vk::DeviceMemory() }, textureImageView{ vk::ImageView() }, sampler{vk::Sampler()}, texWidth{ 0 }, texHeight{ 0 } {}

VTexture::VTexture(VTexture && x)
{
	device = x.device;
	textureImage = x.textureImage;
	textureImageMemory = x.textureImageMemory;
	textureImageView = x.textureImageView;
	sampler = x.sampler;
	texWidth = x.texWidth;
	texHeight = x.texHeight;

	x.device = nullptr;
	x.textureImage = vk::Image();
	x.textureImageMemory = vk::DeviceMemory();
	x.textureImageView = vk::ImageView();
	x.sampler = vk::Sampler();
	x.texHeight = 0;
	x.texWidth = 0;
}

VTexture & VTexture::operator=(VTexture && x)
{
	if (this != &x)
	{
		device = x.device;
		textureImage = x.textureImage;
		textureImageMemory = x.textureImageMemory;
		textureImageView = x.textureImageView;
		sampler = x.sampler;
		texWidth = x.texWidth;
		texHeight = x.texHeight;

		x.device = nullptr;
		x.textureImage = vk::Image();
		x.textureImageMemory = vk::DeviceMemory();
		x.textureImageView = vk::ImageView();
		x.sampler = vk::Sampler();
		x.texHeight = 0;
		x.texWidth = 0;
	}
	return *this;
}


vk::ImageView VTexture::getImageView() const
{
	return textureImageView;
}

vk::Sampler VTexture::getSampler() const
{
	return sampler;
}

uint32_t VTexture::getWidth() const
{
	return texWidth;
}

uint32_t VTexture::getHeight() const
{
	return texHeight;
}

VTexture::~VTexture()
{
	clear();
}

void VTexture::clear()
{
	if (sampler)
	{
		device->destroySampler(sampler);
	}
	if (textureImageView)
	{
		device->destroyImageView(textureImageView);
	}
	if (textureImageMemory)
	{
		device->freeMemory(textureImageMemory);
	}
	if (textureImage)
	{
		device->destroyImage(textureImage);
	}
}
