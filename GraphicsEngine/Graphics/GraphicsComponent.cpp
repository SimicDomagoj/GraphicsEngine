#include "GraphicsComponent.h"
#include"vulkan\vulkan.hpp"
#include"..\DebugTools\Assert.h"
#include"..\Core\VModel.h"

GraphicsComponent::GraphicsComponent(GraphicsComponent && x) : id{ x.id }, layer{ x.layer }, drawType{ x.drawType }, model{ std::move(x.model) }, texture{ std::move(x.texture) }, 
																descriptor{ std::move(x.descriptor) }, uniform{ std::move(x.uniform) }
{
	x.id = -1;
}

GraphicsComponent & GraphicsComponent::operator=(GraphicsComponent && x)
{
	if (this != &x)
	{
		clear();
		id = x.id;
		layer = x.layer;
		drawType = x.drawType;
		model = std::move(x.model);
		texture = std::move(x.texture);
		descriptor = std::move(x.descriptor);
		uniform = std::move(x.uniform);
	}
	return *this;
}

void GraphicsComponent::updateUniform(const glm::mat4& mat)
{
	uniform.updateBuffer(mat);
}

PipelineType GraphicsComponent::getDrawType() const
{
	return drawType;
}

void GraphicsComponent::setDrawType(const PipelineType type)
{
	drawType = type;
}

void GraphicsComponent::draw(const vk::CommandBuffer& buffer, const Pipeline& pipeline) const
{
	ASSERT(*pipeline.layout->getLocalSet() == descriptor.getDescriptorLayout())
	// Bind vertex buffer
	model->vertices.bind(buffer, 0);
	// Bind index buffer
	model->indices.bind(buffer, 0);
	// Bind descriptor set to pipeline
	buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipeline.layout, 1, vk::ArrayProxy<const vk::DescriptorSet>(descriptor), nullptr);
	// Issue a draw call
	buffer.drawIndexed(model->indices.getIndicesCount(), 1, 0, 0, 0);
}

uint32_t GraphicsComponent::getId() const
{
	return id;
}

int32_t GraphicsComponent::getLayer() const
{
	return layer;
}

GraphicsComponent::~GraphicsComponent()
{
	clear();
}

void GraphicsComponent::clear()
{	
}