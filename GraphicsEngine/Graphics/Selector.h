#pragma once
#include"GameObject.h"

/**
	An empty game object representing a 2D selector component.
	Serves as a container for parts of the selector.
*/
class Selector : public GameObject
{
public:
	/**
		Constructor.
	*/
	Selector() {}
	/**
		Move constructor.
	*/
	Selector(Selector&& x);
	/**
		Move assignment operator.
	*/
	Selector& operator=(Selector&& x);
	/**
		Destructor.
	*/
	~Selector();
	friend class ObjectFactory;
protected:
	/**
		Constructor.
	*/
	Selector(GameObject&& x) : GameObject{ std::move(x) } {}
	GameObject* name;								//*< Pointer to a game object that represents the name of the selector.
	GameObject* yesOption;							//*< Pointer to a game object that represents selector's positive option.
	GameObject* noOption;							//*< Pointer to a game object that represents selector's negative option.
	//Note pointer to value structure
};