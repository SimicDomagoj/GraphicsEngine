#pragma once
#include"VertexBuffer.h"
#include"IndexBuffer.h"

/**
	Structure representing a model. Holds the model's vertices and indices stored in the GPU.
*/
struct VModel
{
	/**
		Constructor.
	*/
	VModel();
	VModel(VModel& x) = delete;
	/**
		Move constructor.
	*/
	VModel(VModel&& x);
	VModel& operator=(VModel& x) = delete;
	/**
		Move assignment operator.
	*/
	VModel& operator=(VModel&& x);
	VertexBuffer vertices;			//*< GPU buffer holding model's vertices.
	IndexBuffer indices;			//*< GPU buffer holding model's indices.
};