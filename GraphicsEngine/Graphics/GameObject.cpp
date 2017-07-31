#include "GameObject.h"
#include"..\Graphics\GraphicsComponent.h"
#include "glm\gtc\matrix_transform.hpp"
#include"..\Core\GraphicsEngine.h"
#include"..\DebugTools\Exceptions.h"

uint32_t GameObject::nextId = 0;

GameObject::GameObject()
{
	id = nextId;
	nextId++;
	sceneId = -1;
}

GameObject::GameObject(GameObject && x)
{
	id = x.id;
	sceneId = x.sceneId;
	parent = x.parent;
	graph = x.graph;
	phys = x.phys;
	translationMatrix = x.translationMatrix;
	rotationMatrix = x.rotationMatrix;
	scaleMatrix = x.scaleMatrix;
	children = std::move(x.children);
	engine = x.engine;

	parent = nullptr;
	x.graph = nullptr;
	x.phys = nullptr;
	x.translationMatrix = glm::mat4();
	x.rotationMatrix = glm::mat4();
	x.scaleMatrix = glm::mat4();
	x.engine = nullptr;
}

GameObject & GameObject::operator=(GameObject && x)
{
	if (this != &x)
	{
		clear();
		id = x.id;
		sceneId = x.sceneId;
		parent = x.parent;
		graph = x.graph;
		phys = x.phys;
		translationMatrix = x.translationMatrix;
		rotationMatrix = x.rotationMatrix;
		scaleMatrix = x.scaleMatrix;
		children = std::move(x.children);
		engine = x.engine;

		parent = nullptr;
		x.graph = nullptr;
		x.phys = nullptr;
		x.translationMatrix = glm::mat4();
		x.rotationMatrix = glm::mat4();
		x.scaleMatrix = glm::mat4();
		x.engine = nullptr;
	}
	return *this;
}

glm::mat4 GameObject::getModelMatrix() const
{
	//Calculate the model matrix and return it
	return translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::update(const double time)
{
	// Update physics if object has physics
	if (phys != nullptr)
	{
		phys->update(this, time);
	}
	// Update graphics if object has graphics
	if (graph != nullptr)
	{
		// If object has a parent apply parents model matrix
		glm::mat4 mat = translationMatrix * rotationMatrix * scaleMatrix;
		if (parent != nullptr)
		{
			mat = parent->getModelMatrix() * mat;
		}
		graph->updateUniform(mat);
	}
	//Update each child
	for (GameObject* child : children)
	{
		child->update(time);
	}
}

void GameObject::move(const glm::vec3 & offset)
{
	translationMatrix = glm::translate(translationMatrix, offset);
}

void GameObject::setRotationMatrix(const glm::mat4 & matrix)
{
	rotationMatrix = matrix;
}

glm::vec3 GameObject::getPosition() const
{
	//Last row of the translation matrix corresponds to the position.
	return glm::vec3 { translationMatrix[3] };
}

glm::mat4 GameObject::getRotationMatrix() const
{
	return rotationMatrix;
}

void GameObject::addChild(GameObject * child)
{
	// Add a child and set yourself as it's parent
	children.push_back(child);
	child->setParent(this);
}

std::list<GameObject*> GameObject::getChildren() const
{
	return children;
}

bool GameObject::hasGraphics() const
{
	return graph != nullptr;
}

void GameObject::setParent(GameObject * parent)
{
	this->parent = parent;
}

uint32_t GameObject::getId() const
{
	return id;
}

int GameObject::getLayer() const
{
	if (graph != nullptr)
	{
		return graph->getLayer();
	}
	throw GraphicsException();
}

int32_t GameObject::getSceneId() const
{
	return sceneId;
}

void GameObject::setSceneId(const int32_t sceneId)
{
	this->sceneId = sceneId;
}

GameObject::~GameObject()
{
	clear();
}

void GameObject::clear()
{
	if (phys != nullptr)
	{
		delete phys;
		phys = nullptr;
	}
	if (graph != nullptr)
	{
		engine->deleteObject(id, sceneId);
	}
	graph = nullptr;
}