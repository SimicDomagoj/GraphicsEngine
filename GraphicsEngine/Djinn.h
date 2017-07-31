#pragma once
#include"Scene\Scene.h"

class GraphicsEngine;

class Djinn
{
public:
	void initialize(const char* appName, uint32_t screenWidth, uint32_t screenHeight);
	void setWindowSize(const uint32_t& screenWidth, const uint32_t& screenHeight);
	void setScene(Scene* scene);
	void setObjectLayer(int objectId, int newLayer, int sceneId = -1);
	void run();
	~Djinn();
private:
	void update();
	Scene* scene;
	GraphicsEngine* engine;
};