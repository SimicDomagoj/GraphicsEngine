#pragma once
#include"ResourceManager.h"
#include"..\Core\VTexture.h"

class GraphicsEngine;

/**
	Resource manager class used for loading and storing textures.
*/
class TextureManager : public ResourceManager<VTexture>
{
public:
	/**
		Constructor.
		@param engine pointer to a graphics engine which will be used by manager.
	*/
	TextureManager(const GraphicsEngine* engine);
	/**
		Gets a texture from a collection.
		@param filename filename of the texture to be fetched.
	*/
	std::shared_ptr<VTexture> get(const std::string& filename);
	/**
		Destructor.
	*/
	~TextureManager();
protected:
	/**
		Helper function used for loading a texture from disk.
		@param filename name of the file containing a model.
	*/
	std::shared_ptr<VTexture> load(const std::string& filename);
private:
	const GraphicsEngine* engine;	//*< pointer to a graphics engine used by a manager.
};