#pragma once
#include"..\Core\DynamicBuffer.h"
#include <glm\glm.hpp>
#include"..\Core\DescriptorSet.h"
#include"..\Core\Pipeline.h"
#include"..\Core\PipelineType.h"
#include<memory>

class VTexture;
struct VModel;

/**
	Class used to store information and data needed by the engine to draw a game object.
*/
class GraphicsComponent
{
public:
	/**
		Constructor.
	*/
	GraphicsComponent() {}
	GraphicsComponent(GraphicsComponent& x) = delete;
	/**
		Move constructor.
	*/
	GraphicsComponent(GraphicsComponent&& x);
	GraphicsComponent& operator=(GraphicsComponent& x) = delete;
	/**
		Move assignment operator.
	*/
	GraphicsComponent& operator=(GraphicsComponent&& x);
	/**
		Updates the uniform buffer corresponding to model matrix.
		@param mat new model matrix.
	*/
	void updateUniform(const glm::mat4& mat);
	/**
		Returns the draw type by which the component is drawn.
		@return enumerator describing a draw type.
	*/
	PipelineType getDrawType() const;
	/**
		Sets the way the component will be drawn.
		@param type enumerator describing a draw type.
	*/
	void setDrawType(const PipelineType type);
	/**
		Issues a draw command for drawing a component. Command buffer must be started and graphics pipeline must be bound before issuing this call.
		@param buffer command buffer used for issuing commands.
		@param pipeline graphics pipeline with which the component will be drawn.
	*/
	virtual void draw(const vk::CommandBuffer& buffer, const Pipeline& pipeline) const;
	/**
		Returns component's id.
		@return component's id.
	*/
	uint32_t getId() const;
	/**
		Returns component's layer.
		@return component's layer.
	*/
	int32_t getLayer() const;
	/**
		Destructor.
	*/
	virtual ~GraphicsComponent();
	friend class VulkanEngine;
protected:
	/**
		Clears all neccesary components.
	*/
	virtual void clear();
	uint32_t id;						//*< Component's id which is the same as the id of the corresponding game object.
	int32_t layer;						//*< Component's layer. Components with same layer are drawn together. Layers are drawn in ascending order.
	PipelineType drawType;				//*< Enumerator describing the way the component will be drawn.
	std::shared_ptr<VModel> model;		//*< Pointer to component's model.
	std::shared_ptr<VTexture> texture;	//*< Pointer to component's texture. 
	DescriptorSet descriptor;			//*< Descriptor set which describes a local 
	DynamicBuffer<glm::mat4> uniform;	//*< Buffer used to store component's model transformation.
};