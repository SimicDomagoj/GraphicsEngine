#pragma once
#include"..\Core\DynamicBuffer.h"
#include<glm\glm.hpp>

/**
	Structure containing buffers which store shaders global variable data.
*/
struct GlobalBuffers
{
	/**
		Constructor.
	*/
	GlobalBuffers() {}
	GlobalBuffers(GlobalBuffers& x) = delete;
	/**
		Move constructor.
	*/
	GlobalBuffers(GlobalBuffers&& x) : transform{ std::move(x.transform) }, light{ std::move(x.light) }, camera{ std::move(x.camera) } {}
	GlobalBuffers& operator=(GlobalBuffers& x) = delete;
	/**
		Move assignment operator.
	*/
	GlobalBuffers& operator=(GlobalBuffers&& x)
	{
		transform = std::move(x.transform);
		light = std::move(x.light);
		camera = std::move(x.camera);
		return *this;
	}
	DynamicBuffer<glm::mat4> transform;	//*< Buffer containing projection view transformation.
	DynamicBuffer<glm::vec3> light;		//*< Buffer containing light information.
	DynamicBuffer<glm::vec3> camera;	//*< Buffer containing camera (eye) position.
};