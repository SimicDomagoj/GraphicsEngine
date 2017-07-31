#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "TextureManager.h"
#include "..\Core\GraphicsEngine.h"
#ifdef _DEBUG
#include<iostream>
#endif

TextureManager::TextureManager(const GraphicsEngine* engine)
{
	this->engine = engine;
}

std::shared_ptr<VTexture> TextureManager::get(const std::string& filename)
{
	std::string name = extractName(filename);
	std::shared_ptr<VTexture> resource;
	//Find the resource if it is loaded and return it.
	resource = find(name);
	if (resource != nullptr)
	{
#ifdef _DEBUG
		hit++;
#endif
		return resource;
	}
#ifdef _DEBUG
	miss++;
#endif
	//load and return resource.
	return load(filename);
}

std::shared_ptr<VTexture> TextureManager::load(const std::string& filename)
{
	std::shared_ptr<VTexture> texturePtr;
	int width, height, channels;
	//Load pixels
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}
	//Create a texture
	VTexture tex = engine->createTexture(pixels, width, height);
	stbi_image_free(pixels);

	texturePtr = std::make_shared<VTexture>(std::move(tex));
	//Add texture to collection and return it.
	add(extractName(filename), texturePtr);
	return texturePtr;
}

TextureManager::~TextureManager()
{
#ifdef _DEBUG
	std::cout << "Texture hit: " << hit << std::endl;
	std::cout << "Texture miss: " << miss << std::endl;
#endif
}
