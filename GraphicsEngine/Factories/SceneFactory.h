#pragma once
#include"..\Scene\Scene.h"

/**
	SceneCreate structure.
	Contains information needed to create a scene.
*/
struct SceneCreate
{
	float fieldOfViewDeg; //*< Field of vision of the camera in degrees.
	float nearClip; //*< Distance of a near clipping plane.
	float farClip; //*< Distance of a far clipping plane.
	glm::vec3 cameraPosition; //*< Initial position of a camera.
	glm::vec3 cameraOrientation; //* Initial orientation of the camera.
};

/**
	SceneFactory class
	Creates a scene and different scene objects.
*/
class SceneFactory
{
public:
	/**
		Creates a scene.
		@param params structure with parameters needed to create a scene.
	*/
	static Scene createScene(const SceneCreate& params);
	friend class Djinn;
private:
	static GraphicsEngine* engine; //*< Pointer to a graphics engine used.
};
