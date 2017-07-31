#include "ObjectFactory.h"
#include "..\Graphics\Vertex.h"
#include "glm\gtc\matrix_transform.hpp"
#include "..\Core\GraphicsEngine.h"
#include"..\Scene\Scene.h"
#include"..\Graphics\ModelType.h"
#include"..\Graphics\GraphicsComponent.h"
#include"..\Graphics\Binder.h"
#include"..\Graphics\Selector.h"
#include"..\Graphics\Slider.h"
#include"..\Physics\BillboardRotation.h"
#include"..\Physics\SimpleRotation.h"
#include"..\Physics\SkyBoxMovement.h"

GraphicsEngine* ObjectFactory::engine = nullptr;

GameObject* ObjectFactory::createGameObject(const ObjectCreate & params)
{
	GameObject* go = new GameObject();
	go->engine = engine;
	if (params.normalMap == nullptr && params.depthMap == nullptr)
	{
		go->graph = engine->createGraphicsComponent(go->getId(), params.mesh, params.texture, params.drawType, params.layer, ModelType::e3D);
	}
	else if (params.depthMap == nullptr)
	{
		go->graph = engine->createGraphicsComponent(go->getId(), params.mesh, params.texture, params.normalMap, params.drawType, params.layer);
	}
	else
	{
		go->graph = engine->createGraphicsComponent(go->getId(), params.mesh, params.texture, params.normalMap, params.depthMap, params.drawType, params.layer);
	}
	go->move(params.position);
	go->scaleMatrix = glm::scale(glm::mat4(), params.scale);
	go->rotationMatrix = glm::rotate(glm::mat4(), params.radians, params.rotationAxis);
	go->phys = nullptr;
	go->graph->setDrawType(params.drawType);
	return go;
}

GameObject* ObjectFactory::createEmpty(glm::vec3 position, glm::vec3 scale, float radians, glm::vec3 rotationAxis)
{
	GameObject* go = new GameObject();
	go->engine = engine;
	go->move(position);
	go->scaleMatrix = glm::scale(glm::mat4(), scale);
	go->rotationMatrix = glm::rotate(glm::mat4(), radians, rotationAxis);
	go->phys = nullptr;
	go->graph = nullptr;
	return go;
}

GameObject* ObjectFactory::createGameObjectRotation(const ObjectCreate & params)
{
	GameObject* go = new GameObject();
	go->engine = engine;
	go->graph = engine->createGraphicsComponent(go->getId(), params.mesh, params.texture, params.drawType, params.layer, ModelType::e3D);
	go->move(params.position);
	go->scaleMatrix = glm::scale(glm::mat4(), params.scale);
	go->rotationMatrix = glm::rotate(glm::mat4(), params.radians, params.rotationAxis);
	go->phys = new SimpleRotation();
	go->graph->setDrawType(params.drawType);
	return go;
}

GameObject* ObjectFactory::createSkyBox(Scene* scene, const char * texture)
{
	GameObject* go = new GameObject();
	go->engine = engine;
	go->graph = engine->createGraphicsComponent(go->getId(), "Models/skybox.obj", texture, PipelineType::eSkybox, -1, ModelType::e3D);
	go->move(scene->getCameraPosition());
	go->scaleMatrix = glm::mat4();
	go->rotationMatrix = glm::mat4();
	go->phys = new SkyBoxMovement(scene);
	go->graph->setDrawType(PipelineType::eSkybox);
	return go;
}

GameObject* ObjectFactory::createBillboard(const BillboardCreate& params)
{
	GameObject* go = new GameObject();
	go->engine = engine;
	go->graph = engine->createGraphicsComponent(go->getId(), "Models/plane.obj", params.texture, params.drawType, params.layer, ModelType::e3D);
	go->move(params.position);
	go->scaleMatrix = glm::scale(glm::mat4(), params.scale);
	go->rotationMatrix = glm::rotate(glm::mat4(), params.radians, params.rotationAxis);
	go->phys = new BillboardRotation(params.scene, params.axis);
	go->graph->setDrawType(params.drawType);
	return go;
}

GameObject* ObjectFactory::createSlider(const SliderCreate & params)
{
	glm::vec3 pos, scale;
	pos.x = params.position.x * 2 - 1;
	pos.y = -1 * (params.position.y * 2 - 1);
	scale.x = params.position.z;
	scale.y = params.position.w;
	GameObject* go = createEmpty(pos, scale, 0, glm::vec3{ 1,1,1 });
	Slider* s = new Slider{ std::move(*go) };
	delete go;
	s->line = createMenuItem(glm::vec4{ 0, 0, 1.f, 1.f }, params.lineTexture, PipelineType::eOrthoTextured);
	s->slider = createMenuItem(glm::vec4{ 0, 0, 0.2f, 0.8f }, params.sliderTexture, PipelineType::eOrthoTextured);
	s->addChild(s->line);
	s->addChild(s->slider);
	return s;
}

GameObject* ObjectFactory::createSelector(const SelectorCreate & params)
{
	glm::vec3 pos, scale;
	pos.x = params.position.x * 2 - 1;
	pos.y = -1 * (params.position.y * 2 - 1);
	scale.x = params.position.z;
	scale.y = params.position.w;
	GameObject* go = createEmpty(pos, scale, 0, glm::vec3{ 1,1,1 });
	Selector* s = new Selector{ std::move(*go) };
	delete go;
	glm::vec4 pos2 = params.namePosition;
	pos2.x = pos2.x * 2 - 1;
	pos2.y = -1 * (pos2.y * 2 - 1);
	s->name = createMenuItem(pos2, params.nameTexture, PipelineType::eOrthoTextured);

	pos2 = params.yesPosition;
	pos2.x = pos2.x * 2 - 1;
	pos2.y = -1 * (pos2.y * 2 - 1);
	s->yesOption = createMenuItem(pos2, params.yesTexture, PipelineType::eOrthoTextured);

	pos2 = params.noPosition;
	pos2.x = pos2.x * 2 - 1;
	pos2.y = -1 * (pos2.y * 2 - 1);
	s->noOption = createMenuItem(pos2, params.noTexture, PipelineType::eOrthoTextured);
	s->addChild(s->name);
	s->addChild(s->yesOption);
	s->addChild(s->noOption);
	return s;
}

GameObject* ObjectFactory::createBinder(const BinderCreate & params)
{
	glm::vec3 pos, scale;
	pos.x = params.position.x * 2 - 1;
	pos.y = -1 * (params.position.y * 2 - 1);
	scale.x = params.position.z;
	scale.y = params.position.w;
	GameObject* go = createEmpty(pos,scale, 0, glm::vec3{ 1,1,1 });
	Binder* b = new Binder{ std::move(*go) };
	delete go;
	glm::vec4 pos2 = params.namePosition;
	pos2.x = pos2.x * 2 - 1;
	pos2.y = -1 * (pos2.y * 2 - 1);
	b->name = createMenuItem(pos2, params.nameTexture, PipelineType::eOrthoTextured);

	pos2 = params.qubePosition;
	pos2.x = pos2.x * 2 - 1;
	pos2.y = -1 * (pos2.y * 2 - 1);
	b->qube = createMenuItem(pos2, params.qubeTexture, PipelineType::eOrthoTextured);
	b->addChild(b->name);
	b->addChild(b->qube);
	return b;
}

GameObject* ObjectFactory::createMenuItem(const glm::vec4 & position, const char * texFilename, PipelineType pipeline)
{
	GameObject* item = new GameObject();
	item->engine = engine;
	item->graph = engine->createGraphicsComponent(item->getId(), "Models/plane.obj", texFilename, pipeline, 2, ModelType::e2D);
	item->translationMatrix = glm::translate(glm::mat4(), glm::vec3{ position.x, position.y, 0.f });
	item->scaleMatrix = glm::scale(glm::mat4(), glm::vec3{ position.z, position.w, 0.f });
	item->graph->setDrawType(pipeline);
	return item;
}
