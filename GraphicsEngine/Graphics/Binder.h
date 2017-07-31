#pragma once
#include"GameObject.h"

/**
	An empty game object representing a 2D binder component.
	Serves as a container for parts of the binder.
*/
class Binder : public GameObject
{
public:
	/**
		Constructor.
	*/
	Binder() {}
	/**
		Move constructor.
	*/
	Binder(Binder&& x);
	/**
		Move assignment operator.
	*/
	Binder& operator=(Binder&& x);
	/**
		Destructor.
	*/
	~Binder();
	friend class ObjectFactory;
protected:
	/**
		Constructor.
	*/
	Binder(GameObject&& x) : GameObject{ std::move(x) } {}
	GameObject* name;								//*< Pointer to a game object that represents the name of the binder.
	GameObject* qube;								//*< Pointer to a game object that represents the part of a binder in which the selection is made.
	//NOTE pointer to value structure
};