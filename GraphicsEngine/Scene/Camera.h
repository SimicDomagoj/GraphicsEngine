#pragma once
#include"glm\glm.hpp"
#include"glm\gtc\matrix_transform.hpp"

/** 
	A camera class.
	Represents the view origin of the scene and defines a view and projection transformations
*/
class Camera
{
public:
	/**
		Default constructor
	*/
	Camera();
	/**
		Constructor with custom starting position and orientation and other camera characteristics.
		@param position starting position of the camera.
		@param orientation starting orientation of the camera.
		@param aspectRatio camera's aspect ratio.
		@param FOV camera's field of vision expressed in degrees.
		@param nearClip distance to a plane in front of which nothing is seen.
		@param farClip distance to a plane behind which nothing is seen.
	*/
	Camera(const glm::vec3& position, const glm::vec3& orientation, const float aspectRatio, const float FOV, const float nearClip, const float farClip);
	/**
		Copy constructor.
	*/
	Camera(Camera& x);
	/**
		Move constructor.
	*/
	Camera(Camera&& x);
	/**
		Copy assignment operator.
	*/
	Camera& operator=(Camera& x);
	/**
		Move assignment operator.
	*/
	Camera& operator=(Camera&& x);
	/**
		Moves the camera for the desired offset by it's local coordinate system.
		@param translation offset by which camera is moved.
	*/
	void move(const glm::vec3& translation);
	/**
		Rotates the camera around it's local axes. Axis will be normalized.
		@param axis combination of local axis around which camera will be rotated.
		@param radians angle for which the camera will be rotated, expresed in radians.
	*/
	void rotate(const glm::vec3& axis, const float radians);
	/**
		Zoom the view for given amount of units. One unit increases or decreases field of vision by one degree.
		@param units amount of units by which the filed of vision will be modified;
	*/
	void zoom(const float units);
	/**
		Sets camera's aspect ratio.
		@param ratio camera's new aspect ratio.
	*/
	void setAspectRatio(const float ratio);
	/**
		Sets camera's aspect ratio.
		@param width width used to calculate aspect ratio.
		@param height height used to calculate aspect ratio.
	*/
	void setAspectRatio(const uint32_t width, const uint32_t height);
	/**
		Returns the matrix which contains the transformation from world-space to camera's view-space.
		@return view transformation matrix.
	*/
	glm::mat4 getViewMatrix() const;
	/**
		Returns the matrix which contains the transformation from view-space to projection-space.
		@return view transformation matrix.
	*/
	glm::mat4 getProjectionMatrix() const;
	/**
		Returns the current position of the camera.
		@return position of the camera.
	*/
	glm::vec3 getPosition() const;
	/**
		Destructor.
	*/
	~Camera();
private:
	glm::vec3 position;		//*< A three dimensional vector variable used to store the position of the camera.
	glm::vec3 orientation;	//*< A three dimensional vector variable used to store the orientation of the camera. Vector is normalized.
	glm::vec3 upVector;		//*< A three dimensional vector variable used to store camera's up vector. Vector is normalized.
	float aspectRatio;		//*< Float variable used to store camera's aspect ratio.
	float FOV;				//*< Float variable used to store camera's field of vision expressed in degrees. Value will always be in interval of [0.1, 150] degrees.
	float nearClipPlane;	//*< Float variable used to store camera's near clipping plane.
	float farClipPlane;		//*< Float variable used to store camera's far clipping plane.
};