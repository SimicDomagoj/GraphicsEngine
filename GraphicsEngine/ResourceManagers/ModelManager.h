#pragma once
#include"ResourceManager.h"
#include"..\Core\VModel.h"
#include"..\Graphics\ModelType.h"

class GraphicsEngine;

/**
	Resource manager class used for loading and storing models.
*/
class ModelManager : public ResourceManager<VModel>
{
public:
	/**
		Constructor.
		@param engine pointer to a graphics engine which will be used by manager.
	*/
	ModelManager(const GraphicsEngine* engine);
	/**
		Gets a model from a colection.
		@param filename filename of the model to be fetched.
		@param type type of a model we want to load.
	*/
	std::shared_ptr<VModel> get(const std::string& filename, const ModelType& type);
	/**
		Destructor.
	*/
	~ModelManager();
protected:
	/**
		Helper function used for loading a 3D model from disk.
		@param filename name of the file containing a model.
	*/
	std::shared_ptr<VModel> load3D(const std::string& filename);
	/**
		Helper function used for loading a 3D model from disk and calculating its tangents and bitangents.
		@param filename name of the file containing a model.
	*/
	std::shared_ptr<VModel> load3DWithTangent(const std::string& filename);
	/**
		Helper function used for loading a 2D model from disk.
		@param filename name of the file containing a model.
	*/
	std::shared_ptr<VModel> load2D(const std::string& filename);
private:
	const GraphicsEngine* engine;	//*< pointer to a graphics engine used by a manager.
};