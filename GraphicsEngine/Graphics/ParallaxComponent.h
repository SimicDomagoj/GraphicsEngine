#pragma once
#include"GraphicsComponent.h"

/**
	Class inheriting GraphicsComponent class.
	Class used to store information and data needed by the engine to draw a game object with extra textures serving as a bump and depth maps.
*/
class ParallaxComponent : public GraphicsComponent
{
public:
	/**
		Consturctor.
	*/
	ParallaxComponent() {}
	/**
		Move constructor.
	*/
	ParallaxComponent(ParallaxComponent&& x);
	/**
		Move assignment operator.
	*/
	ParallaxComponent& operator=(ParallaxComponent&& x);
	/**
		Destructor.
	*/
	~ParallaxComponent();
	friend VulkanEngine;
protected:
	/**
		Clears all neccesary components.
	*/
	virtual void clear() override;
	std::shared_ptr<VTexture> normalMap;	//*< Pointer to component's normal map.
	std::shared_ptr<VTexture> depthMap;		//*< Pointer to component's depth map.
};