#include"ParallaxComponent.h"

ParallaxComponent::ParallaxComponent(ParallaxComponent && x) : GraphicsComponent(std::move(x)), normalMap{ std::move(x.normalMap) }, depthMap{ std::move(x.depthMap) } {}

ParallaxComponent & ParallaxComponent::operator=(ParallaxComponent && x)
{
	if (this != &x)
	{
		clear();
		GraphicsComponent::operator=(std::move(x));
		normalMap = std::move(x.normalMap);
		depthMap = std::move(x.depthMap);
	}
	return *this;
}

ParallaxComponent::~ParallaxComponent()
{
	clear();
	GraphicsComponent::clear();
}

void ParallaxComponent::clear()
{
}