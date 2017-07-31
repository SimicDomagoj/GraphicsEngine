#pragma once
#include"glm\glm.hpp"

class GameObject;

/**
	PhysicsComponent abstract class.
	Base abstract class for physics behavior.
*/
class PhysicsComponent
{
public:
	/**
		Updates the designated GameObject.
		@param obj pointer to object which to update.
		@param time time since last global update.
	*/
	virtual void update(GameObject* obj, const double time) = 0;
};