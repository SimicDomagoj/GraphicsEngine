#pragma once
#include"GraphicsComponent.h"

/**
	Class inheriting GraphicsComponent class.
	Class used to store information and data needed by the engine to draw a game object with extra texture serving as a bump map.
*/
class BumpMapComponent : public GraphicsComponent
{
public:
	/**
		Consturctor.
	*/
	BumpMapComponent() {}
	/**
		Move constructor.
	*/
	BumpMapComponent(BumpMapComponent&& x);
	/**
		Move assignment operator.
	*/
	BumpMapComponent& operator=(BumpMapComponent&& x);
	/**
		Destructor.
	*/
	~BumpMapComponent();
	friend VulkanEngine;
protected:
	/**
		Clears all neccesary components.
	*/
	virtual void clear() override;
	std::shared_ptr<VTexture> normalMap;	//*< Pointer to component's normal map.
};