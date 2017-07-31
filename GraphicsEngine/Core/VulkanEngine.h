#pragma once
#include"VulkanBase.h"
#include"PipelineType.h"
#include"DescriptorSet.h"
#include"..\Graphics\GlobalBuffers.h"
#include"..\Graphics\SceneGraphics.h"
#include<memory>
#include"..\ResourceManagers\TextureManager.h"
#include"..\ResourceManagers\ModelManager.h"
#include"..\DebugTools\Result.h"

/**
	Implementation of the graphics engine using Vulkan API
*/
class VulkanEngine : public VulkanBase
{
public:
	/**
		Constructor.
	*/
	VulkanEngine();
	VulkanEngine(VulkanEngine const&) = delete;
	void operator=(VulkanEngine const&) = delete;
	/**
		Creates a graphics components, for desired game object, which holds all neccessary information to draw an object.
		@param id id of the game object for which we create a component. Id will be assigned to a component as well so it has a connection to a game object.
		Uniqueness of the id needs to be handeled externally, otherwise problems may arrise if there happen to be components with same ids.
		@param model filename of the model used.
		@param texFilename filename of the texture used.
		@param pipeline enumerator which determines how we want to draw the object.
		@param layer layer to which the object is assigned.
		@param loadType type of the model we want to use.
		@return shared pointer to the component created with given parameters.
	*/
	std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, PipelineType pipeline, int layer, ModelType loadType) override;
	/**
		Creates a graphics components, for desired game object, which holds all neccessary information to draw an object.
		@param id id of the game object for which we create a component. Id will be assigned to a component as well so it has a connection to a game object.
		Uniqueness of the id needs to be handeled externally, otherwise problems may arrise if there happen to be components with same ids.
		@param model filename of the model used.
		@param texFilename filename of the texture used.
		@param normalMap filename of the normal map used.
		@param pipeline enumerator which determines how we want to draw the object.
		@param layer layer to which the object is assigned.
		@return shared pointer to the component created with given parameters.
	*/
	std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, const char* normalMap, PipelineType pipeline, int layer) override;
	/**
		Creates a graphics components, for desired game object, which holds all neccessary information to draw an object.
		@param id id of the game object for which we create a component. Id will be assigned to a component as well so it has a connection to a game object.
		Uniqueness of the id needs to be handeled externally, otherwise problems may arrise if there happen to be components with same ids.
		@param model filename of the model used.
		@param texFilename filename of the texture used.
		@param normalMap filename of the normal map used.
		@param depthMap filename of the depth map used.
		@param pipeline enumerator which determines how we want to draw the object.
		@param layer layer to which the object is assigned.
		@return shared pointer to the component created with given parameters.
	*/
	std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, const char* normalMap, const char* depthMap, PipelineType pipeline, int layer) override;
	/**
		Signals the engine to update all internal data related to the given scene so it can be drawn.
		Doesn't need to be called every frame, it can be called when a change occurs.
		@param sceneId id of the scene we want to update and draw next.
	*/
	void update(int sceneId) override;
	/**
		Creates buffers primarily used by the scenes to store global information needed to draw objects such as projection view transformation and such.
		@return GlobalBuffers structure which holds all neccessary buffers.
	*/
	GlobalBuffers createGlobalBuffers() override;
	/**
		Registers a new scene. Creates data needed by the engine for internal representation of the scene and assignes an id to the scene.
		@param buffer reference to scene's buffers used to create internal representation of the scene. Buffers every scene needs have for it to be drawn by the engine.
		@return id assigned to the scene.
	*/
	int registerScene(const GlobalBuffers& buffers) override;
	/**
		Deregisters a scene. Deletes all data related to the scene used by the engine.
		@param id id of the scene we want to deregister.
	*/
	void deRegisterScene(int id) override;
	/**
		Attaches an object to the scene.
		@param sceneId id of the scene to which we want to attach the object.
		@param objectId id of the object we want to attach.
	*/
	void attachObject(int sceneId, int objectId) override;
	/**
		Detaches an object from the scene.
		@param sceneId id of the scene in which the object is currently contained.
		@param objectId id of the object which we want to detach.
	*/
	void detachObject(int sceneId, int objectId) override;
	/**
		Deletes all data, corresponding to the desired object, used by the engine.
		@param objectId id of the object which we want to delete.
		@param sceneId id of the scene in which the object is contained. -1 if object is not contained in the scene.
	*/
	void deleteObject(int objectId, int sceneId = -1) override;
	/**
		Sets the layer for the given object.
		@param objectId id of the object whose layer we want to set.
		@param newLayer layer which to assign to the object.
		@param sceneId id of the scene in which the object is contained. -1 if object is not contained in the scene.
	*/
	void setObjectLayer(int objectId, int newLayer, int sceneId = -1) override;
	/**
		Checks if the window in which we are drawing is currently active.
		@return true if window is active, false otherwise.
	*/
	bool isWindowActive() const override;
	/**
		Tells the engine we are finished working with it, so it can clean up everything it needs to.
		Needs to be called when we are finished working with it.
	*/
	void finish() override;
	/**
		Draws current scene to the screen.
		Generaly should be called once per frame.
	*/
	void draw() override;
	/**
		Destructor.
	*/
	virtual ~VulkanEngine();
protected:
	/**
		Creates a render pass. Render pass contains the information about framebuffer attachments.
		For example how many color and depth buffer are attached to it and how many samples are use for each of them.
		Also is holds the information how the contents of those buffers should be handled throughout the rendering operations.
	*/
	virtual void createRenderPass() override;
	/**
		Creates descriptor set layout. Descriptor layout specifies the types of resources that a certain set in the shader has.
		For example it describes how many uniform variables, samplers or other variables a shader set has.
	*/
	virtual void createDescriptorSetLayout() override;
	/**
		Creates graphic pipelines used by the engine.
	*/
	virtual void createGraphicsPipeline() override;
	/**
		Creates a pool out of which descriptors are allocated.
	*/
	virtual void createDescriptorPool() override;
	/**
		Recreates swapchain and all other neccessary components which need to be recreated when swapchain is recreated.
	*/
	virtual void recreateSwapChain() override;
	/**
		Gets a pipeline which corresponds to enumerator describing the pipeline.
		@param pipeline enumerator describing the pipeline.
		@return Pipeline object.
	*/
	Pipeline getPipeline(PipelineType pipeline);

	std::vector<vk::CommandBuffer> commandBuffers;						//*< Vector of command buffers used to issued commands.
	std::vector<SceneGraphics> scenes;									//*< Vector of objects which contain all information engine needs about a scene.
	std::list<std::shared_ptr<GraphicsComponent>> unassignedComponents;	//*< List of components which are currently not in any scene.
	TextureManager textureManager;										//*< Resource manager used to load textures.
	ModelManager modelManager;											//*< Resource manager used to load models.
private:
	/**
		Creates descriptor sets which describe global(same for all models) shader variables and links
		those sets to desired buffers.
		@param buffers buffers used to store global shader variables.
		@return vector of created descriptor sets.
	*/
	std::vector<DescriptorSet> createGlobalDescriptors(const GlobalBuffers& buffers);
	/**
		Transfers an object from one list to another.
		@param from list from which to transfer the object.
		@param to list to which to transfer the object.
		@param objectId id of the object to transfer.
		@return Result enum. eSucces if transfered successfully. eNotFound if object was not found in a given list.
	*/
	Result transferObject(std::list<std::shared_ptr<GraphicsComponent>>& from, std::list<std::shared_ptr<GraphicsComponent>>& to, int objectId);
	/**
		Finds the iterator in the list at which the object needs to be placed. 
		@param list list in which we want to place the object.
		@item item which we want to place in the list.
	*/
	std::list<std::shared_ptr<GraphicsComponent>>::const_iterator findInsertIterator(const std::list<std::shared_ptr<GraphicsComponent>>& list, const std::shared_ptr<GraphicsComponent>& item) const;
};
