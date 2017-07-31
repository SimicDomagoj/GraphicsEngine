#include"Slider.h"

Slider::Slider(Slider && x) : GameObject{ std::move(x) }, line{ std::move(x.line) }, slider{ std::move(x.slider) } {}

Slider & Slider::operator=(Slider && x)
{
	if (this != &x)
	{
		GameObject::operator=(std::move(x));
		line = std::move(x.line);
		slider = std::move(x.slider);
	}
	return *this;
}

Slider::~Slider()
{
	delete slider;
	slider = nullptr;
	delete line;
	line = nullptr;
}