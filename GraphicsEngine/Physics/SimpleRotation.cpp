#include"SimpleRotation.h"
#include"..\Graphics\GameObject.h"
#include"glm\gtc\matrix_transform.hpp"

void SimpleRotation::update(GameObject * obj, const double time)
{
	//rotate the object
	glm::mat4 rotMatrix = obj->getRotationMatrix();
	rotMatrix = glm::rotate(rotMatrix, static_cast<float>(time * glm::radians(90.0f)), glm::vec3(0.f, 1.0f, 1.0f));
	obj->setRotationMatrix(rotMatrix);
}