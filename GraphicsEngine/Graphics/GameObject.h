#pragma once
#include<glm\glm.hpp>
#include"..\Physics\PhysicsComponent.h"
#include<memory>
#include<list>

class GraphicsComponent;
class GraphicsEngine;

/**
	Class containing all relevant information and data needed for a game object.
	Users should note that certain variables are accessible only if the object has graphics.
	If object does not have graphics, trying to access those variable will result in exception.
*/
class GameObject
{
public:
	/**
		Constructor.
	*/
	GameObject();
	GameObject(GameObject& x) = delete;
	/**
		Move constructor.
	*/
	GameObject(GameObject&& x);
	GameObject& operator=(GameObject& x) = delete;
	/**
		Move assignment operator.
	*/
	GameObject& operator=(GameObject&& x);
	/**
		Returns object's model transformation matrix.
		@return model transformation matrix.
	*/
	glm::mat4 getModelMatrix() const;
	/**
		Updates the game object and all of it's children.
		@param time time passed since last update call.
	*/
	virtual void update(const double time);
	/**
		Move's the object by an offset.
		@param offset offset by which to move the object.
	*/
	void move(const glm::vec3& offset);
	/**
		Sets objects rotation matrix.
		@param matrix new rotation matrix.
	*/
	void setRotationMatrix(const glm::mat4& matrix);
	/**
		Return object's position.
		@return object's position.
	*/
	glm::vec3 getPosition() const;
	/**
		Returns object's rotation matrix.
		@return object's rotation matrix.
	*/
	glm::mat4 getRotationMatrix() const;
	/**
		Add a child game object to this object.
		@param child pointer to a game object which to add as a child element.
	*/
	void addChild(GameObject* child);
	/**
		Returns object's child objects.
		@return list of child objects.
	*/
	std::list<GameObject*> getChildren() const;
	/**
		Checks if the object has a graphics component.
		@return true if object has a graphics components. False otherwise.
	*/
	bool hasGraphics() const;
	/**
		Returns object's id.
		@return object's id.
	*/
	uint32_t getId() const;
	/**
		Returns object's layer. If object does not have graphics this throw GraphicsException.
		@return object's layer.
		@throw GraphicsException object does not have graphics.
	*/
	int32_t getLayer() const;
	/**
		Returns id of the scene in which the object is contained.
		@return id of the scene in which the object is contained.
		@return -1 if object is not contained in a scene.
	*/
	int32_t getSceneId() const;
	/**
		Sets id of a scene in which the object is contained.
		@param sceneId id of a scene in which the object is contained.
	*/
	void setSceneId(const int32_t sceneId);
	/**
		Destructor.
	*/
	virtual ~GameObject();
	friend class ObjectFactory;
protected:
	/**
		Clears all neccesary components.
	*/
	void clear();
	/**
		Sets the parent object.
		@param parent pointer to the parent game object.
	*/
	void setParent(GameObject* parent);
	uint32_t id;								//*< Object's id.
	int32_t sceneId;							//*< Id of the scene in which the object is contained.
	GameObject* parent{ nullptr };				//*< Pointer to the parent object.
	std::shared_ptr<GraphicsComponent> graph;	//*< Pointer to object's graphics component.
	PhysicsComponent* phys;						//*< Pointer to object's physics component.
	glm::mat4 translationMatrix{ glm::mat4() };	//*< Object's translation transform matrix.
	glm::mat4 rotationMatrix;					//*< Object's rotation transform matrix.
	glm::mat4 scaleMatrix;						//*< Object's scale transform matrix.
	std::list<GameObject*> children;			//*< List containing object's child elements.
private:
	GraphicsEngine* engine;						//*< Pointer to graphics engine used by object.
	static uint32_t nextId;						//*< Static variable used to store id of the object that will be created next.
};