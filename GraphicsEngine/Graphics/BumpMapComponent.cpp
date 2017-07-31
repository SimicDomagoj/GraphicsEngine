#include"BumpMapComponent.h"

BumpMapComponent::BumpMapComponent(BumpMapComponent && x) : GraphicsComponent(std::move(x)), normalMap{ std::move(x.normalMap) } {}

BumpMapComponent & BumpMapComponent::operator=(BumpMapComponent && x)
{
	if (this != &x)
	{
		clear();
		GraphicsComponent::operator=(std::move(x));
		normalMap = std::move(x.normalMap);
	}
	return *this;
}

BumpMapComponent::~BumpMapComponent()
{
	clear();
	GraphicsComponent::clear();
}

void BumpMapComponent::clear()
{
}