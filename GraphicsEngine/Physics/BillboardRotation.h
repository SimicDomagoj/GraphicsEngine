#pragma once
#include"PhysicsComponent.h"
#include"..\Graphics\Axis.h"

class Scene;

/**
	BillboardRotation class.
	Rotates an object so that it always faces the camera.
*/
class BillboardRotation : public PhysicsComponent
{
public:
	/**
	Constructor.
		@param scene scene's camera orientation is used to rotate an object.
		@param axis determines around which axis billboarding will take effect.
	*/
	BillboardRotation(const Scene* scene, const Axis axis);
	/**
		Updates the designated GameObject.
		@param obj pointer to object which to update.
		@param time time since last global update.
	*/
	virtual void update(GameObject * obj, const double time) override;
private:
	const Scene* scene;		//*< Pointer to a Scene object.
	glm::vec3 axisMask;		//*< 3D vector used to determine around which axis we need to rotate the object.
};