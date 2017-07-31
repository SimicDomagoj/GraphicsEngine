#include"SkyBoxMovement.h"
#include"..\Graphics\GameObject.h"
#include"..\Scene\Scene.h"

void SkyBoxMovement::update(GameObject * obj, const double time)
{
	//get the distance between camera and object
	glm::vec3 dist = scene->getCameraPosition() - obj->getPosition();
	//move object.
	obj->move(dist);
}