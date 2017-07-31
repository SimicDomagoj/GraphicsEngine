#pragma once
#include<list>
#include"..\Core\DescriptorSet.h"
#include"GraphicsComponent.h"

/**
	Structure that holds information needed by the graphics engine to draw the scene.
*/
struct SceneGraphics
{
	int32_t id{ -1 };										//*< Scene's id.
	std::vector<DescriptorSet> descriptors;					//*< Descriptor sets containing global shader sets.
	std::list<std::shared_ptr<GraphicsComponent>> items;	//*< Items contained in the scene.
};