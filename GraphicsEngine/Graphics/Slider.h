#pragma once
#include"GameObject.h"

/**
	An empty game object representing a 2D slider component.
	Serves as a container for parts of the slider.
*/
class Slider : public GameObject
{
public:
	/**
		Constructor.
	*/
	Slider() {}
	/**
		Move constructor.
	*/
	Slider(Slider&& x);
	/**
		Move assignment operator.
	*/
	Slider& operator=(Slider&& x);
	/**
		Destructor.
	*/
	~Slider();
	friend class ObjectFactory;
protected:
	/**
		Constructor.
	*/
	Slider(GameObject&& x) : GameObject{ std::move(x) } {}
	GameObject* line;							//*< Pointer to a game object that represents the line part of the slider.
	GameObject* slider;							//*< Pointer to a game object that represents the movable part of the slider.
	//NOTE pointer to value structure
};