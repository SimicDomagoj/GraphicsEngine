#include "Djinn.h"
#include"Core/VulkanEngine.h"
#include<chrono>
#include"Factories\SceneFactory.h"
#include"Factories\ObjectFactory.h"

#ifdef _DEBUG
	static bool validating = true;
#else
	static bool validating = false;
#endif

void Djinn::initialize(const char* appName, uint32_t screenWidth, uint32_t screenHeight)
{
	VulkanEngine* e = new VulkanEngine();
	e->init(appName, validating, screenWidth, screenHeight);
	engine = e;
	SceneFactory::engine = engine;
	ObjectFactory::engine = engine;
}

void Djinn::setWindowSize(const uint32_t& screenWidth, const uint32_t& screenHeight)
{
	engine->setWindowSize(screenWidth, screenHeight);
	if (scene != nullptr)
	{
		scene->setScreenSize(screenWidth, screenHeight);
	}
}

void Djinn::setScene(Scene* scene)
{
	this->scene = scene;
	glm::vec2 screenSize = engine->getScreenSize();
	scene->setScreenSize(static_cast<uint32_t>(screenSize.x), static_cast<uint32_t>(screenSize.y));
}

void Djinn::setObjectLayer(int objectId, int newLayer, int sceneId)
{
	engine->setObjectLayer(objectId, newLayer, sceneId);
}

void Djinn::run()
{
	while (engine->isWindowActive() && scene != nullptr)
	{
		glfwPollEvents();
		int state = glfwGetKey(engine->getWindow(), GLFW_KEY_K);
		if (state == GLFW_PRESS)
		{
			setObjectLayer(16, -1, 0);
		}
		engine->update(scene->getId());
		update();
		engine->draw();
	}
	engine->finish();
}

Djinn::~Djinn()
{
	delete engine;
	engine = nullptr;
}

void Djinn::update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
	startTime = std::chrono::high_resolution_clock::now();

	scene->update(time);
	
}
