#pragma once
#include"PhysicsComponent.h"

/**
	SimpleRotation class.
	Constantly rotates an object by a fixed angle.
*/
class SimpleRotation : public PhysicsComponent
{
	/**
		Updates the designated GameObject.
		@param obj pointer to object which to update.
		@param time time since last global update.
	*/
	virtual void update(GameObject * obj, const double time) override;
};