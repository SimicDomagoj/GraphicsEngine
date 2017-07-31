#include "SceneFactory.h"
#include "..\Core\GraphicsEngine.h"

GraphicsEngine* SceneFactory::engine = nullptr;

Scene SceneFactory::createScene(const SceneCreate & params)
{
	Scene s;
	s.engine = engine;
	s.window = engine->getWindow();
	glm::vec2 windowDims = engine->getScreenSize();
	Camera camera{ params.cameraPosition, params.cameraOrientation, windowDims.x / (float)windowDims.y, params.fieldOfViewDeg, params.nearClip, params.farClip };
	s.camera = camera;
	s.buffers = engine->createGlobalBuffers();
	s.id = engine->registerScene(s.buffers);
	return s;
}

