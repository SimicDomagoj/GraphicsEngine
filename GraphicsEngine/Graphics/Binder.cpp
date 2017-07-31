#include"Binder.h"

Binder::Binder(Binder && x) : GameObject{ std::move(x) }, name{ std::move(x.name) }, qube{ std::move(x.qube) } {}

Binder & Binder::operator=(Binder && x)
{
	if (this != &x)
	{
		GameObject::operator=(std::move(x));
		name = std::move(x.name);
		qube = std::move(x.qube);
	}
	return *this;
}

Binder::~Binder()
{
	delete qube;
	qube = nullptr;
	delete name;
	name = nullptr;
}