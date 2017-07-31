#include<iostream>
#include"Factories\ObjectFactory.h"
#include"Factories\SceneFactory.h"
#include"Djinn.h"

int main()
{
	Djinn djinn;
	djinn.initialize("Djinn", 800, 600);
	GameObject* slider = ObjectFactory::createSlider(SliderCreate{ glm::vec4{ 0.1f, 0.1f, 0.15f, 0.15f}, "Textures/slider.png", "Textures/littleSlider.png", "" });
	GameObject* binder = ObjectFactory::createBinder(BinderCreate{ glm::vec4{ 0.4f, 0.4f, 0.3f, 0.1f }, glm::vec4{ 0.f, 0.5f, 0.4f, 1.f }, glm::vec4{ 0.6f, 0.5f, 0.4f, 1.f }, "Textures/name.png", "Textures/box.png", "" });
	GameObject* selector = ObjectFactory::createSelector(SelectorCreate{ glm::vec4{ 0.15f, 0.8f, 0.3f, 0.3f }, glm::vec4{ 0.5f, 0.75f, 0.4f, 0.5f }, glm::vec4{ 0.25f, 0.25f, 0.4f, 0.5f },
											glm::vec4{ 0.75f, 0.25f, 0.4f, 0.5f }, "Textures/name.png", "Textures/yes.png", "Textures/no.png", "" });
	Scene s = SceneFactory::createScene(SceneCreate{ 45.f, 0.1f, 50.f, glm::vec3{0.0, 0.0, 1.0}, glm::vec3{0.0, 0.0, -1.0} });
	s.addLight(glm::vec3{ 5.0, 0.0, 5.0 });
	GameObject* object = ObjectFactory::createGameObjectRotation(ObjectCreate{ glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.25, 0.25, 0.25), 0, glm::vec3(0.0, 0.0, 1.0), "Models/cube.obj", "Textures/cube.png", nullptr, nullptr, PipelineType::ePhong, 0 });
	s.addItem(object);
	GameObject* object1 = ObjectFactory::createGameObjectRotation(ObjectCreate{ glm::vec3(0.5, 0.5, -5.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/cube.obj", "Textures/cube.png", nullptr, nullptr, PipelineType::eWireframe, 0 });
	s.addItem(object1);
	GameObject* object2 = ObjectFactory::createGameObjectRotation(ObjectCreate{ glm::vec3(0.5, -0.2, -5.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/cube.obj", "Textures/cube.png", nullptr, nullptr, PipelineType::ePhong, 0 });
	s.addItem(object2);
	GameObject* object3 = ObjectFactory::createGameObjectRotation(ObjectCreate{ glm::vec3(-0.5, 0.3, -5.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/cube.obj", "Textures/cube.png", nullptr, nullptr, PipelineType::eToon, 0 });
	s.addItem(object3);
	GameObject* object4 = ObjectFactory::createGameObjectRotation(ObjectCreate{ glm::vec3(-0.8, -0.3, -2.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/cube.obj", "Textures/cube.png", nullptr, nullptr, PipelineType::eNoLight, 0 });
	s.addItem(object4);
	GameObject* billboard = ObjectFactory::createBillboard(BillboardCreate{ &s, Axis::eY, glm::vec3{0.0, 0.0, 2.0}, glm::vec3{ 1.0, 1.0, 1.0 }, 1.0, glm::vec3(0.0, 0.0, 1.0), "Textures/no.png", PipelineType::eNoLight,1 });
	s.addItem(billboard);
	GameObject* bump = ObjectFactory::createGameObject(ObjectCreate{ glm::vec3(2.0, 0.0, -1.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/plane.obj", "Textures/bricks2.jpg", "Textures/bricks2N.jpg", nullptr, PipelineType::eBumpMap, 0 });
	s.addItem(bump);
	GameObject* notBump = ObjectFactory::createGameObject(ObjectCreate{ glm::vec3(1.75, 0.0, -1.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/plane.obj", "Textures/bricks2.jpg", nullptr, nullptr, PipelineType::ePhong, 0 });
	s.addItem(notBump);
	GameObject* bump2 = ObjectFactory::createGameObject(ObjectCreate{ glm::vec3(2.5, 0.0, -1.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/plane.obj", "Textures/dirt.jpg", "Textures/dirtN.jpg", nullptr, PipelineType::eBumpMap, 0 });
	s.addItem(bump2);
	GameObject* notBump2 = ObjectFactory::createGameObject(ObjectCreate{ glm::vec3(2.25, 0.0, -1.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/plane.obj", "Textures/dirt.jpg", nullptr, nullptr, PipelineType::ePhong, 0 });
	s.addItem(notBump2);
	GameObject* parallax = ObjectFactory::createGameObject(ObjectCreate{ glm::vec3(1.5, 0.0, -1.0), glm::vec3(0.125, 0.125, 0.125), 0, glm::vec3(0.0, 0.0, 1.0), "Models/plane.obj", "Textures/bricks2.jpg", "Textures/bricks2N.jpg", "Textures/bricks2D.jpg", PipelineType::eParallax, 0 });
	s.addItem(parallax);
	GameObject* skybox = ObjectFactory::createSkyBox(&s, "Textures/SkyBox.png");
	s.addItem(skybox);
	s.addItem(slider);
	s.addItem(binder);
	s.addItem(selector);
	djinn.setScene(&s);
	try
	{
		djinn.run();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	delete skybox;
	delete parallax;
	delete notBump2;
	delete bump2;
	delete notBump;
	delete bump;
	delete billboard;
	delete object4;
	delete object3;
	delete object2;
	delete object1;
	delete object;
	delete selector;
	delete binder;
	delete slider;

	return EXIT_SUCCESS;
}