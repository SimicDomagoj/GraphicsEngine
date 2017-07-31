#pragma once
#include"PhysicsComponent.h"

class Scene;

/**
	SkyBoxMovement class.
	Constantly moves an object so that it is always in the same place as the scene's camera.
*/
class SkyBoxMovement : public PhysicsComponent
{
public:
	/**
		Constructor.
		@param scene scene whose camera's position is used to move the object.
	*/
	SkyBoxMovement(const Scene* scene) : scene{ scene } {}
	/**
		Updates the designated GameObject.
		@param obj pointer to object which to update.
		@param time time since last global update.
	*/
	virtual void update(GameObject * obj, const double time) override;
private:
	const Scene* scene;		//*< Pointer to a Scene object.
};