#pragma once
#include<vector>
#include"..\Graphics\GameObject.h"
#include<GLFW\glfw3.h>
#include"Camera.h"
#include"..\Graphics\GlobalBuffers.h"
#include"..\DebugTools\Result.h"

/**
	Class that represents a scene.
*/
class Scene
{
public:
	/**
		Default constructor.
	*/
	Scene() {}
	Scene(Scene& x) = delete;
	/**
		Move constructor.
	*/
	Scene(Scene&& x);
	Scene& operator=(Scene& x) = delete;
	/**
		Move assignment operator.
	*/
	Scene& operator=(Scene&& x);
	/**
		Set dimensions of the window on which the scene will be drawn
		@param width width of the window
		@param height height of the window
	*/
	void setScreenSize(const uint32_t width, const uint32_t height);
	/**
		Adds an object to the scene.
		@param item pointer to the item to be added to scene.
	*/
	Result addItem(GameObject* item);
	/**
		Remove an object from the scene.
		@param item pointer to the item to be removed from scene.
		@return Success if item was successfuly added.
		@return AssignedElsewhere if item was not added because it is contained in another scene.
	*/
	Result removeItem(GameObject* item);
	/**
		Adds an light to the scene.
		@param origin origin of the point light to be added to scene.
		@return Success if item was successfuly added.
		@return AssignedElsewhere if item is contained in another scene.
		@return NotFound if item was not found in the scene.
	*/
	void addLight(const glm::vec3 origin);
	/**
		Updates the scene and all of it's objects.
		@param time amount of time which passed since last update.
	*/
	void update(const double time);
	/** 
		Returns scene's id.
		@return scene's id.
	*/
	uint32_t getId() const;
	/**
		Returns the amount of light contained in the scene.
		@return number of lights in the scene.
	*/
	uint32_t getNumLights() const;
	/**
		Returns the current position of the scene's camera.
		@return position of the camera.
	*/
	glm::vec3 getCameraPosition() const;
	/**
		Destructor.
	*/
	virtual ~Scene();
	friend class SceneFactory;
protected:
	uint32_t id;							//*< Scene's id assigned by the engine.
	GraphicsEngine* engine;			//*< Pointer to a graphics engine.
	GLFWwindow* window;				//*< Window in which the scene is rendered.
	Camera camera;					//*< Scene's camera.
	GlobalBuffers buffers;			//*< Buffers which hold global data that are common to all objects.
	std::vector<glm::vec3> lights;	//*< Array of 3D vector variables. Contains sources of point light in the scene.
	std::vector<GameObject*> items;	//*< Array of pointers to GameObject objects. Contains all items contained in a scene.
};