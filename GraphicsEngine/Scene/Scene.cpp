#include"Scene.h"
#include"glm\gtc\matrix_transform.hpp"
#include"..\Core\GraphicsEngine.h"
#include<queue>

const float cameraSpeed = 1.f;		//*< Camera's movement speed.
const float cameraRotation = 1.f;	//*< Camera's rotation speed.
const float cameraZoom = 10.f;		//*< Camera's zoom speed.

Scene::Scene(Scene && x) : id{ std::move(x.id) }, engine{ std::move(x.engine) }, window{ x.window }, camera{ x.camera },
							buffers{ std::move(x.buffers) }, lights{ std::move(x.lights) }, items{ std::move(x.items) } 
{
	x.id = -1;
	x.engine = nullptr;
}

Scene & Scene::operator=(Scene && x)
{
	if (this != &x)
	{
		id = x.id;
		engine = std::move(x.engine);
		window = x.window;
		camera = x.camera;
		buffers = std::move(x.buffers);
		lights = std::move(x.lights);
		items = std::move(x.items);

		engine = nullptr;
		x.id = -1;
	}
	return *this;
}

void Scene::setScreenSize(const uint32_t width, const uint32_t height)
{
	camera.setAspectRatio(width, height);
}

Result Scene::addItem(GameObject * item)
{
	if (item->getSceneId() != -1)
	{
		return Result::eAssignedElsewhere;
	}
	item->setSceneId(id);
	//add item to array.
	items.push_back(item);
	std::queue<GameObject*> queue;
	queue.push(item);
	std::list<GameObject*> children;
	// Attach object and all of its children to a scene
	while (queue.empty() == false)
	{
		GameObject* go = queue.front();
		children = go->getChildren();
		for (GameObject* listObj : children)
		{
			queue.push(listObj);
		}
		if (go->hasGraphics())
		{
			engine->attachObject(id, go->getId());
		}
		queue.pop();
	}
	return Result::eSuccess;
}

Result Scene::removeItem(GameObject * item)
{
	if (item->getSceneId() != id)
	{
		return Result::eAssignedElsewhere;
	}
	std::vector<GameObject*>::iterator it;
	it = std::find(items.begin(), items.end(), item);
	if (it == items.end())
	{
		return Result::eNotFound;
	}
	// Remove object from collection
	items.erase(it);
	std::queue<GameObject*> queue;
	queue.push(item);
	std::list<GameObject*> children;
	// Remove object and all of its children from the scene
	while (queue.empty() == false)
	{
		GameObject* go = queue.front();
		go->setSceneId(-1);
		children = go->getChildren();
		for (GameObject* listObj : children)
		{
			queue.push(listObj);
		}
		if (go->hasGraphics())
		{
			engine->detachObject(id, go->getId());
		}
		queue.pop();
	}
	return Result::eSuccess;
}

void Scene::addLight(const glm::vec3 origin)
{
	//add light to array.
	lights.push_back(origin);
	//update light buffer.
	buffers.light.updateBuffer(lights[0]);
}

void Scene::update(const double time)
{
	//handle camera movement and rotation.
	int state = glfwGetKey(window, GLFW_KEY_W);
	if (state == GLFW_PRESS)
	{
		camera.move(glm::vec3{ 0,0,(float)time * cameraSpeed });
	}
	state = glfwGetKey(window, GLFW_KEY_S);
	if (state == GLFW_PRESS)
	{
		camera.move(glm::vec3{ 0,0,(float)time * -cameraSpeed });
	}
	state = glfwGetKey(window, GLFW_KEY_A);
	if (state == GLFW_PRESS)
	{
		camera.move(glm::vec3{ (float)time * -cameraSpeed, 0, 0 });
	}
	state = glfwGetKey(window, GLFW_KEY_D);
	if (state == GLFW_PRESS)
	{
		camera.move(glm::vec3{ (float)time * cameraSpeed, 0, 0 });
	}
	state = glfwGetKey(window, GLFW_KEY_E);
	if (state == GLFW_PRESS)
	{
		camera.rotate(glm::vec3{ 0,1,0 }, (float)time * -cameraRotation);
	}
	state = glfwGetKey(window, GLFW_KEY_Q);
	if (state == GLFW_PRESS)
	{
		camera.rotate(glm::vec3{ 0,1,0 }, (float)time * cameraRotation);
	}
	state = glfwGetKey(window, GLFW_KEY_R);
	if (state == GLFW_PRESS)
	{
		camera.rotate(glm::vec3{ 1,0,0 }, (float)time * cameraRotation);
	}
	state = glfwGetKey(window, GLFW_KEY_F);
	if (state == GLFW_PRESS)
	{
		camera.rotate(glm::vec3{ 1,0,0 }, (float)time * -cameraRotation);
	}
	state = glfwGetKey(window, GLFW_KEY_KP_SUBTRACT);
	if (state == GLFW_PRESS)
	{
		camera.zoom((float)time * cameraZoom);
	}
	state = glfwGetKey(window, GLFW_KEY_KP_ADD);
	if (state == GLFW_PRESS)
	{
		camera.zoom((float)time * -cameraZoom);
	}
	buffers.transform.updateBuffer(camera.getProjectionMatrix() * camera.getViewMatrix());
	buffers.camera.updateBuffer(camera.getPosition());
	for (GameObject* object : items)
	{
		object->update(time);
	}
}

uint32_t Scene::getId() const
{
	return id;
}

uint32_t Scene::getNumLights() const
{
	return lights.size();
}

glm::vec3 Scene::getCameraPosition() const
{
	return camera.getPosition();
}

Scene::~Scene()
{
	if (id != -1)
	{
		for (GameObject* item : items)
		{
			item->setSceneId(-1);
		}
		engine->deRegisterScene(id);
	}
}
