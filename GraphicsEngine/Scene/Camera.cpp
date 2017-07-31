#include "Camera.h"
#include "glm\gtx\rotate_vector.hpp"
#include "..\Core\Constants.h"

Camera::Camera() : position{ 0,0,0 }, orientation{ -cnst::forwardVector }, upVector{ cnst::upVector }, aspectRatio{ 1.f }, FOV{ 45.f }, nearClipPlane{ 0 }, farClipPlane{ 10.f } {}

Camera::Camera(const glm::vec3 & position, const glm::vec3 & orientation, const float aspectRatio, const float FOV, const float nearClip, const float farClip)
	: position{ position }, orientation{ glm::normalize(orientation) }, upVector{ cnst::upVector }, aspectRatio{ aspectRatio }, FOV{ FOV }, nearClipPlane{ nearClip }, farClipPlane{ farClip }{}

Camera::Camera(Camera & x) : position{ x.position }, orientation{ x.orientation }, upVector{ x.upVector }, 
					aspectRatio{ x.aspectRatio }, FOV{ x.FOV }, nearClipPlane{ x.nearClipPlane }, farClipPlane{ x.farClipPlane } {}

Camera::Camera(Camera && x) : position{ std::move(x.position) }, orientation{ std::move(x.orientation) }, upVector{ std::move(x.upVector) },
					aspectRatio{ std::move(x.aspectRatio) }, FOV{ std::move(x.FOV) }, nearClipPlane{ std::move(x.nearClipPlane) }, farClipPlane{ std::move(x.farClipPlane) }{}

Camera & Camera::operator=(Camera & x)
{
	if (this != &x)
	{
		position = x.position;
		orientation = x.orientation;
		upVector = x.upVector;
		aspectRatio = x.aspectRatio;
		FOV = x.FOV;
		nearClipPlane = x.nearClipPlane;
		farClipPlane = x.farClipPlane;
	}
	return *this;
}

Camera & Camera::operator=(Camera && x)
{
	if (this != &x)
	{
		position = std::move(x.position);
		orientation = std::move(x.orientation);
		upVector = std::move(x.upVector);
		aspectRatio = std::move(x.aspectRatio);
		FOV = std::move(x.FOV);
		nearClipPlane = std::move(x.nearClipPlane);
		farClipPlane = std::move(x.farClipPlane);
	}
	return *this;
}

void Camera::move(const glm::vec3 & translation)
{
	//calculate camera's right vector
	glm::vec3 rightVector = glm::cross(orientation, upVector);
	//move the camera
	position += translation.x * rightVector;
	position += translation.y * upVector;
	position += translation.z * orientation;
}

void Camera::rotate(const glm::vec3& axis, const float radians)
{
	//calculate the camera's right vector
	glm::vec3 rightVector = glm::cross(orientation, upVector);
	//normalize the rotation axis vector
	glm::vec3 normAxis = glm::normalize(axis);
	//calculate the local rotation axis
	glm::vec3 rotAxis = normAxis.x * rightVector + normAxis.y * upVector + normAxis.z * orientation;
	//rotate the camera
	orientation = glm::rotate(orientation, radians, rotAxis);
	upVector = glm::rotate(upVector, radians, rotAxis);
}

void Camera::zoom(const float units)
{
	FOV += units;
	// We limit field of vision to an interval of [0.1, 150].
	if (FOV < 0.1f)
	{
		FOV = 0.1f;
	}
	else if (FOV > 150.f)
	{
		FOV = 150.f;
	}
}

void Camera::setAspectRatio(const float ratio)
{
	aspectRatio = ratio;
}

void Camera::setAspectRatio(const uint32_t width, const uint32_t height)
{
	aspectRatio = width / static_cast<float>(height);
}

glm::mat4 Camera::getViewMatrix() const
{
	//calculate and return view matrix
	return glm::lookAt(position, position + orientation, upVector); //NOTE maybe store viewMatrix if this is called more often than it is updated
}

glm::mat4 Camera::getProjectionMatrix() const
{
	//calculate and return projection matrix
	glm::mat4 proj = glm::perspective(glm::radians(FOV), aspectRatio, nearClipPlane, farClipPlane); //NOTE maybe store matrix if this is called more often than it is updated
	//GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted. 
	//To compensate for that we flip the sign on the scaling factor of the Y axis in the projection matrix.
	//If we don't do this, the image will be rendered upside down.
	proj[1][1] *= -1;
	return proj;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

Camera::~Camera()
{
}

