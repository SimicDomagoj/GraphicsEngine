#include"Selector.h"

Selector::Selector(Selector && x) : GameObject{ std::move(x) }, name{ std::move(x.name) }, yesOption{ std::move(x.yesOption) }, noOption{ std::move(x.noOption) } {}

Selector& Selector::operator=(Selector && x)
{
	if (this != &x)
	{
		GameObject::operator=(std::move(x));
		name = std::move(x.name);
		yesOption = std::move(x.yesOption);
		noOption = std::move(x.noOption);
	}
	return *this;
}

Selector::~Selector()
{
	delete noOption;
	noOption = nullptr;
	delete yesOption;
	yesOption = nullptr;
	delete name;
	name = nullptr;
}