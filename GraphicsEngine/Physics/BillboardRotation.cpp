#include "BillboardRotation.h"
#include"..\Graphics\GameObject.h"
#include"..\Scene\Scene.h"
#include"..\DebugTools\Assert.h"
#include"..\Core\Constants.h"

BillboardRotation::BillboardRotation(const Scene * scene, const Axis axis) : scene{ scene }
{
	axisMask = glm::vec3{ 1,1,1 };
	// if we rotate around X axis we take in consideration only Y and Z axis when calculating
	// distance between object and camera so we set axis mask in regards to that.
	// in the same way we set axis mask if we are rotating around some other axis.
	if (axis == Axis::eX)
	{
		axisMask.x = 0.0;
	}
	else if (axis == Axis::eY)
	{
		axisMask.y = 0.0;
	}
	else if (axis == Axis::eZ)
	{
		axisMask.z = 0.0;
	}
}

void BillboardRotation::update(GameObject * obj, const double time)
{
	glm::vec3 camPos = scene->getCameraPosition();
	glm::vec3 objPos = obj->getPosition();

	// if camera and object are in the same position, we don't rotate the object
	if (camPos == objPos)
	{
		return;
	}	
	//get the vector from camera to object.
	glm::vec3 objToCamVector = camPos - objPos;
	//apply the axis mask and normalize the vector
	//mask extracts coordinates which we are rotated.
	objToCamVector = glm::normalize(axisMask * objToCamVector);

	//calculate objects forward vector and mask it.
	glm::mat4 rotMatrix = obj->getRotationMatrix();
	glm::vec4 forward = rotMatrix * cnst::forwardVector;
	glm::vec3 forwardMasked = axisMask * glm::vec3{ forward.x, forward.y, forward.z };

	//if lenght of objects masked forward vector is zero, there is no sutable rotation in regards to current position.
	if (glm::length(forwardMasked) == 0.0)
	{
		return;
	}
	//normalize masked forward vector.
	forwardMasked = glm::normalize(forwardMasked);

	//calculate the angle between camera and object.
	float cosine = glm::dot(forwardMasked, objToCamVector);

	glm::vec3 rotationVector;
	// if cosine is almost one, the angle is zero. No rotation is required
	if (cosine > 0.999)
	{
		return;
	}
	// if cosine is around -1 we set it to -1 because of precision issues cosine can sometimes be bigger than 1 and smaller than -1.
	// also we manually set rotation vector because we can't calculate it with cross product because vectors have 180 degrees between them.
	else if (cosine < -0.999)
	{
		cosine = -1.f;
		rotationVector = glm::vec3{ 1,1,1 } - axisMask;
	}
	// if cosine is not 1 or -1 we calculate rotation vector with cross product.
	else
	{
		rotationVector = glm::cross(forwardMasked, objToCamVector);
		rotationVector = glm::normalize(rotationVector);
	}
	// calculate new rotation matrix and assign it to the object.
	float radians = glm::acos(cosine);
	glm::mat4 newMatrix = glm::rotate(glm::mat4(), radians, rotationVector);
	obj->setRotationMatrix(newMatrix * rotMatrix);

	ASSERT(!std::isnan(obj->getRotationMatrix()[0][0]))
}