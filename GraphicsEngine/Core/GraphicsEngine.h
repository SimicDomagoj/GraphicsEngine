#pragma once
#include"glm\glm.hpp"
#include<vector>
#include"GLFW\glfw3.h"
#include<memory>

class VTexture;
class IndexBuffer;
class VertexBuffer;
struct GlobalBuffers;
class GraphicsComponent;
enum class PipelineType;
enum class ModelType;

/**
	Interface with all functions an impelementation of graphics engine should implement.	
*/
class GraphicsEngine
{
public:
	/**
		Returns the screen size.
		@return screen size as two dimensional vector. First value is width, and second is height.
	*/
	virtual glm::vec2 getScreenSize() const = 0;
	/**
		Sets the size of the screen.
		@param width screen's new width.
		@param height screen's new height.
	*/
	virtual void setWindowSize(uint32_t width, uint32_t height) = 0;
	/**
		Creates a vertex buffer. Buffer used by the GPU which stores vertices.
		@param data void pointer to an array of vertices in an arbitrary vertex format.
		@param bufferSize size of data contained in array pointed to by \p data pointer.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return VertexBuffer object created with given parameters.
	*/
	virtual VertexBuffer createVertexBuffer(void* data, const size_t& bufferSize, bool useStaging = true) const = 0;
	/**
		Creates an index buffer. Buffer used by the GPU which stores indices.
		@param indices void pointer to a vector of indices.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return IndexBuffer object created with given parameters.
	*/
	virtual IndexBuffer createIndexBuffer(const std::vector<uint32_t>& indices, bool useStaging = true) const = 0;
	/**
		Creates a texture used by GPU.
		@param pixels pointer to an array of pixels. Data needs to be in 4 channel format.
		@param width texture's width.
		@param height texture's height.
		@param useStaging flag used to determine if we should try to enhance performane in creation of the buffer or not.
		@return VTexture object containing created texture stored in the GPU.
	*/
	virtual VTexture createTexture(unsigned char* pixels, unsigned int width, unsigned int height, bool useStaging = true) const = 0;
	/**
		Returns a pointer to the window in which we draw scenes.
		@return pointer to the currently used window.
	*/
	virtual GLFWwindow* getWindow() const = 0;
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
	virtual std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, PipelineType pipeline, int layer, ModelType loadType) = 0;
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
	virtual std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, const char* normalMap, PipelineType pipeline, int layer) = 0;
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
	virtual std::shared_ptr<GraphicsComponent> createGraphicsComponent(int id, const char * model, const char * texFilename, const char* normalMap, const char* depthMap, PipelineType pipeline, int layer) = 0;
	/**
		Signals the engine to update all internal data related to the given scene so it can be drawn.
		Doesn't need to be called every frame, it can be called when a change occurs.
		@param sceneId id of the scene we want to update and draw next.
	*/
	virtual void update(int sceneId) = 0;
	/**
		Creates buffers primarily used by the scenes to store global information needed to draw objects such as projection view transformation and such.
		@return GlobalBuffers structure which holds all neccessary buffers.
	*/
	virtual GlobalBuffers createGlobalBuffers() = 0;
	/**
		Registers a new scene. Creates data needed by the engine for internal representation of the scene and assignes an id to the scene.
		@param buffer reference to scene's buffers used to create internal representation of the scene. Buffers every scene needs have for it to be drawn by the engine.
		@return id assigned to the scene.
	*/
	virtual int registerScene(const GlobalBuffers& buffers) = 0;
	/**
		Deregisters a scene. Deletes all data related to the scene used by the engine.
		@param id id of the scene we want to deregister.
	*/
	virtual void deRegisterScene(int id) = 0;
	/**
		Attaches an object to the scene.
		@param sceneId id of the scene to which we want to attach the object.
		@param objectId id of the object we want to attach.
	*/
	virtual void attachObject(int sceneId, int objectId) = 0;
	/**
		Detaches an object from the scene.
		@param sceneId id of the scene in which the object is currently contained.
		@param objectId id of the object which we want to detach.
	*/
	virtual void detachObject(int sceneId, int objectId) = 0;
	/**
		Deletes all data, corresponding to the desired object, used by the engine.
		@param objectId id of the object which we want to delete.
		@param sceneId id of the scene in which the object is contained. -1 if object is not contained in the scene.
	*/
	virtual void deleteObject(int objectId, int sceneId = -1) = 0;
	/**
		Sets the layer for the given object.
		@param objectId id of the object whose layer we want to set.
		@param newLayer layer which to assign to the object.
		@param sceneId id of the scene in which the object is contained. -1 if object is not contained in the scene.
	*/
	virtual void setObjectLayer(int objectId, int newLayer, int sceneId = -1) = 0;
	/**
		Checks if the window in which we are drawing is currently active.
		@return true if window is active, false otherwise.
	*/
	virtual bool isWindowActive() const = 0;
	/**
		Tells the engine we are finished working with it, so it can clean up everything it needs to.
		Needs to be called when we are finished working with it.
	*/
	virtual void finish() = 0;
	/**
		Draws current scene to the screen.
		Generaly should be called once per frame.
	*/
	virtual void draw() = 0;
	/**
		Destructor.
	*/
	virtual ~GraphicsEngine() {}
};