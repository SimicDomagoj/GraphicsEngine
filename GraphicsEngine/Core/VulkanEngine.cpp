#include "VulkanEngine.h"
#include "Shader.h"
#include<glm\gtc\matrix_transform.hpp>
#include<chrono>
#include"..\DebugTools\Assert.h"
#include"..\Graphics\Vertex.h"
#include<tiny_obj_loader.h>
#include<unordered_map>
#include"..\Graphics\BumpMapComponent.h"
#include"..\Graphics\ParallaxComponent.h"

VulkanEngine::VulkanEngine() : textureManager{ this }, modelManager{ this } {}

std::shared_ptr<GraphicsComponent> VulkanEngine::createGraphicsComponent(int id, const char * model, const char * texFilename, PipelineType pipeline, int layer, ModelType loadType)
{
	Pipeline graphPipeline = getPipeline(pipeline);
	std::shared_ptr<GraphicsComponent> item = std::make_shared<GraphicsComponent>();
	item->uniform = createDynamicBuffer<glm::mat4>();

	item->model = modelManager.get(std::string(model), loadType);
	item->texture = textureManager.get(std::string(texFilename));
	item->layer = layer;

	vk::DescriptorSetLayout layouts[] = { *graphPipeline.layout->getLocalSet() };
	vk::DescriptorSetAllocateInfo allocInfo{ descriptorPool, 1, layouts };
	
	item->descriptor = DescriptorSet{ logicDevice.allocateDescriptorSets(allocInfo)[0], graphPipeline.layout->getLocalSet(), ShaderUsage::VS_ModelTransform | ShaderUsage::FS_Texture };
	item->descriptor.setDestructor(&logicDevice, &descriptorPool);
	item->id = id;
	unassignedComponents.push_back(item);

	vk::DescriptorBufferInfo bufferInfo{ item->uniform, 0, item->uniform.size() };

	vk::DescriptorImageInfo imageInfo{ item->texture->getSampler(), item->texture->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };

	std::vector<vk::WriteDescriptorSet> descriptorWrites{
		vk::WriteDescriptorSet{ item->descriptor, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWrites, nullptr);
	return item;
}

std::shared_ptr<GraphicsComponent> VulkanEngine::createGraphicsComponent(int id, const char * model, const char * texFilename, const char * normalMap, PipelineType pipeline, int layer)
{
	Pipeline graphPipeline = getPipeline(pipeline);
	std::shared_ptr<BumpMapComponent> item = std::make_shared<BumpMapComponent>();
	item->uniform = createDynamicBuffer<glm::mat4>();

	item->model = modelManager.get(std::string(model), ModelType::e3DTangent);
	item->texture = textureManager.get(std::string(texFilename));
	item->normalMap = textureManager.get(std::string(normalMap));
	item->layer = layer;

	vk::DescriptorSetLayout layouts[] = { *graphPipeline.layout->getLocalSet() };
	vk::DescriptorSetAllocateInfo allocInfo{ descriptorPool, 1, layouts };
	
	item->descriptor = DescriptorSet{ logicDevice.allocateDescriptorSets(allocInfo)[0], graphPipeline.layout->getLocalSet(), ShaderUsage::VS_ModelTransform | ShaderUsage::FS_Texture | ShaderUsage::FS_NormalMap | ShaderUsage::VS_Tangents };
	item->descriptor.setDestructor(&logicDevice, &descriptorPool);
	item->id = id;
	unassignedComponents.push_back(item);

	ASSERT(graphPipeline.localReq == item->descriptor.getUsage())

	vk::DescriptorBufferInfo bufferInfo{ item->uniform, 0, item->uniform.size() };
	vk::DescriptorImageInfo imageInfo{ item->texture->getSampler(), item->texture->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };
	vk::DescriptorImageInfo normalMapInfo{ item->normalMap->getSampler(), item->normalMap->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };

	std::vector<vk::WriteDescriptorSet> descriptorWrites{
		vk::WriteDescriptorSet{ item->descriptor, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 2, 0, 1, vk::DescriptorType::eCombinedImageSampler, &normalMapInfo, nullptr, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWrites, nullptr);
	return item;
}

std::shared_ptr<GraphicsComponent> VulkanEngine::createGraphicsComponent(int id, const char * model, const char * texFilename, const char * normalMap, const char * depthMap, PipelineType pipeline, int layer)
{
	Pipeline graphPipeline = getPipeline(pipeline);
	std::shared_ptr<ParallaxComponent> item = std::make_shared<ParallaxComponent>();
	item->uniform = createDynamicBuffer<glm::mat4>();

	item->model = modelManager.get(std::string(model), ModelType::e3DTangent);
	item->texture = textureManager.get(std::string(texFilename));
	item->normalMap = textureManager.get(std::string(normalMap));
	item->depthMap = textureManager.get(std::string(depthMap));
	item->layer = layer;

	vk::DescriptorSetLayout layouts[] = { *graphPipeline.layout->getLocalSet() };
	vk::DescriptorSetAllocateInfo allocInfo{ descriptorPool, 1, layouts };
	
	item->descriptor = DescriptorSet{ logicDevice.allocateDescriptorSets(allocInfo)[0], graphPipeline.layout->getLocalSet(), ShaderUsage::VS_ModelTransform | ShaderUsage::FS_Texture | ShaderUsage::FS_NormalMap | ShaderUsage::VS_Tangents | ShaderUsage::FS_DepthMap};
	item->descriptor.setDestructor(&logicDevice, &descriptorPool);
	item->id = id;
	unassignedComponents.push_back(item);

	ASSERT(graphPipeline.localReq == item->descriptor.getUsage())

	vk::DescriptorBufferInfo bufferInfo{ item->uniform, 0, item->uniform.size() };
	vk::DescriptorImageInfo imageInfo{ item->texture->getSampler(), item->texture->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };
	vk::DescriptorImageInfo normalMapInfo{ item->normalMap->getSampler(), item->normalMap->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };
	vk::DescriptorImageInfo depthMapInfo{ item->depthMap->getSampler(), item->depthMap->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal };

	std::vector<vk::WriteDescriptorSet> descriptorWrites{
		vk::WriteDescriptorSet{ item->descriptor, 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 2, 0, 1, vk::DescriptorType::eCombinedImageSampler, &normalMapInfo, nullptr, nullptr },
		vk::WriteDescriptorSet{ item->descriptor, 3, 0, 1, vk::DescriptorType::eCombinedImageSampler, &depthMapInfo, nullptr, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWrites, nullptr);
	return item;
}

void VulkanEngine::update(int sceneId)
{
	if (commandBuffers.size() > 0)
	{
		logicDevice.freeCommandBuffers(commandPool, commandBuffers);
	}
	ASSERT(scenes[sceneId].id == sceneId)
	std::vector<DescriptorSet>& sets = scenes[sceneId].descriptors; 
	std::list<std::shared_ptr<GraphicsComponent>>& components = scenes[sceneId].items;
	if (components.size() == 0)
	{
		return;
	}

	vk::CommandBufferAllocateInfo bufferInfo{ commandPool, vk::CommandBufferLevel::ePrimary, swapFramebuffers.size() };
	commandBuffers = logicDevice.allocateCommandBuffers(bufferInfo);

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr };

		commandBuffers[i].begin(beginInfo);

		std::array<vk::ClearValue, 2> clearValues{ vk::ClearColorValue{ std::array<float, 4>{0.1f, 0.1f, 0.1f, 1.0f} }, vk::ClearDepthStencilValue{ 1.0f, 0 } };
		vk::RenderPassBeginInfo renderPassInfo{ renderPass, swapFramebuffers[i], vk::Rect2D{ { 0,0 }, swapExtent }, clearValues.size(), clearValues.data() };

		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

		PipelineType current = components.front()->getDrawType();
		int index = static_cast<int>(current);

		std::vector<DescriptorSet>::iterator it;
		for (it = sets.begin(); it != sets.end(); it++)
		{
			if (it->getUsage() == graphicsPipelines[index].globalReq && it->getDescriptorLayout() == *graphicsPipelines[index].layout->getGlobalSet())
			{
				break;
			}
		}
		ASSERT(it != sets.end())

		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipelines[index].handle);
		commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *graphicsPipelines[index].layout, 0, vk::ArrayProxy<const vk::DescriptorSet>(*it), nullptr);
		for (const std::shared_ptr<GraphicsComponent> component : components)
		{
			if (component->getDrawType() != current)
			{
				current = component->getDrawType();
				index = static_cast<int>(current);

				std::vector<DescriptorSet>::iterator it;
				for (it = sets.begin(); it != sets.end(); it++)
				{
					if (it->getUsage() == graphicsPipelines[index].globalReq && it->getDescriptorLayout() == *graphicsPipelines[index].layout->getGlobalSet())
					{
						break;
					}
				}

				commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipelines[index].handle);
				if (it != sets.end())
				{
					commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *graphicsPipelines[index].layout, 0, vk::ArrayProxy<const vk::DescriptorSet>(*it), nullptr);
				}
			}
			component->draw(commandBuffers[i], graphicsPipelines[index]);
		}
		commandBuffers[i].endRenderPass();
		commandBuffers[i].end();
	}
}

GlobalBuffers VulkanEngine::createGlobalBuffers()
{
	GlobalBuffers buffers;
	buffers.transform = createDynamicBuffer<glm::mat4>();
	buffers.light = createDynamicBuffer<glm::vec3>();
	buffers.camera = createDynamicBuffer<glm::vec3>();
	return buffers;
}

std::vector<DescriptorSet> VulkanEngine::createGlobalDescriptors(const GlobalBuffers & buffers)
{
	std::vector<DescriptorSet> sets{ 3 };
	//Create first set with only pv transform.
	vk::DescriptorSetLayout layouts[] = { descriptorLayouts[0] };
	vk::DescriptorSetAllocateInfo allocInfo{ descriptorPool, 1, layouts };
	sets[0] = DescriptorSet(logicDevice.allocateDescriptorSets(allocInfo)[0], &descriptorLayouts[0], ShaderUsage::VS_PVTransform);
	sets[0].setDestructor(&logicDevice, &descriptorPool);

	vk::DescriptorBufferInfo transformInfo{ buffers.transform, 0, buffers.transform.size() };
	vk::DescriptorBufferInfo lightInfo{ buffers.light, 0, buffers.light.size() };
	vk::DescriptorBufferInfo cameraInfo{ buffers.camera, 0, buffers.camera.size() };

	std::vector<vk::WriteDescriptorSet> descriptorWrites{
		vk::WriteDescriptorSet{ sets[0], 0,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &transformInfo, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWrites, nullptr);

	//Create second set with pv transform and light.
	layouts[0] = descriptorLayouts[2];
	sets[1]  = DescriptorSet(logicDevice.allocateDescriptorSets(allocInfo)[0], &descriptorLayouts[2], ShaderUsage::VS_PVTransform | ShaderUsage::VS_Light);
	sets[1].setDestructor(&logicDevice, &descriptorPool);

	std::vector<vk::WriteDescriptorSet> descriptorWritesLight{
		vk::WriteDescriptorSet{ sets[1], 0,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &transformInfo, nullptr },
		vk::WriteDescriptorSet{ sets[1], 1,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &lightInfo, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWritesLight, nullptr);

	//Create second set with pv transform, light and camera position.
	layouts[0] = descriptorLayouts[4];
	sets[2] = DescriptorSet(logicDevice.allocateDescriptorSets(allocInfo)[0], &descriptorLayouts[4], ShaderUsage::VS_PVTransform | ShaderUsage::VS_Light | ShaderUsage::VS_CameraPos);
	sets[2].setDestructor(&logicDevice, &descriptorPool);

	std::vector<vk::WriteDescriptorSet> descriptorWritesCamera{
		vk::WriteDescriptorSet{ sets[2], 0,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &transformInfo, nullptr },
		vk::WriteDescriptorSet{ sets[2], 1,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &lightInfo, nullptr },
		vk::WriteDescriptorSet{ sets[2], 2,0,1,vk::DescriptorType::eUniformBuffer, nullptr, &cameraInfo, nullptr } };

	logicDevice.updateDescriptorSets(descriptorWritesCamera, nullptr);

	return sets;
}

int VulkanEngine::registerScene(const GlobalBuffers & buffers)
{
	int id;
	std::vector<SceneGraphics>::iterator it = std::find_if(scenes.begin(), scenes.end(), [](SceneGraphics& x) { return x.id == -1; });
	id = it - scenes.begin();
	if (id == scenes.size())
	{
		scenes.push_back(SceneGraphics());
	}
	scenes[id].id = id;
	scenes[id].descriptors = createGlobalDescriptors(buffers);
	return id;
}

void VulkanEngine::deRegisterScene(int id)
{
	scenes[id].id = -1;
	scenes[id].descriptors.clear();
	//When scene is deleted all remaining object are move to unassigned list
	unassignedComponents.splice(unassignedComponents.end(), scenes[id].items, scenes[id].items.begin(), scenes[id].items.end());
	ASSERT(scenes[id].items.size() == 0)
}

void VulkanEngine::attachObject(int sceneId, int objectId)
{
	ASSERT(sceneId < static_cast<int>(scenes.size()) && sceneId >= 0)
	transferObject(unassignedComponents, scenes[sceneId].items, objectId);
}

void VulkanEngine::detachObject(int sceneId, int objectId)
{
	ASSERT(sceneId >= 0 && sceneId < static_cast<int>(scenes.size()))
	transferObject(scenes[sceneId].items, unassignedComponents, objectId);
}

void VulkanEngine::deleteObject(int objectId, int sceneId)
{
	ASSERT(sceneId >= -1 && sceneId < static_cast<int>(scenes.size()))
	if (sceneId == -1)
	{
		unassignedComponents.remove_if([objectId](std::shared_ptr<GraphicsComponent> x) { return x->getId() == objectId; });
	}
	else
	{
		scenes[sceneId].items.remove_if([objectId](std::shared_ptr<GraphicsComponent> x) { return x->getId() == objectId; });
	}
}

void VulkanEngine::setObjectLayer(int objectId, int newLayer, int sceneId)
{
	ASSERT(sceneId >= -1 && sceneId < static_cast<int>(scenes.size()))
	std::shared_ptr<GraphicsComponent> item;
	std::list<std::shared_ptr<GraphicsComponent>>* list;
	std::list<std::shared_ptr<GraphicsComponent>>::iterator it;
	if (sceneId == -1)
	{
		list = &unassignedComponents;
	}
	else
	{
		list = &scenes[sceneId].items;
	}
	it = std::find_if(list->begin(), list->end(), [objectId](std::shared_ptr<GraphicsComponent> x) { return x->getId() == objectId; });
	item = *it;
	list->erase(it);
	item->layer = newLayer;
	list->insert(findInsertIterator(*list, item), item);
}

bool VulkanEngine::isWindowActive() const
{
	return !glfwWindowShouldClose(window);
}

void VulkanEngine::finish()
{
	logicDevice.waitIdle();
}

VulkanEngine::~VulkanEngine()
{
}

void VulkanEngine::createRenderPass()
{
	if (renderPass)
	{
		logicDevice.destroyRenderPass(renderPass);
	}

	vk::AttachmentDescription attachment{vk::AttachmentDescriptionFlags(), swapFormat, vk::SampleCountFlagBits::e1,
											vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
											vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
											vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR };

	vk::AttachmentDescription depthAttachment{ vk::AttachmentDescriptionFlags(), findDepthFormat(), vk::SampleCountFlagBits::e1,
											vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare,
											vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
											vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal };

	vk::AttachmentReference attReference{ 0, vk::ImageLayout::eColorAttachmentOptimal };

	vk::AttachmentReference depthAttachmentRef{ 1, vk::ImageLayout::eDepthStencilAttachmentOptimal };

	vk::SubpassDescription subpass{ vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics, 0, nullptr,
									1, &attReference, nullptr, &depthAttachmentRef, 0, nullptr };

	vk::SubpassDependency dependency{ VK_SUBPASS_EXTERNAL ,0,vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
									vk::AccessFlags(), vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite };

	std::array<vk::AttachmentDescription, 2> attachments = { attachment, depthAttachment };
	vk::RenderPassCreateInfo renderPassInfo{ vk::RenderPassCreateFlags(), attachments.size(), attachments.data(),
											1, &subpass, 1, &dependency };

	renderPass = logicDevice.createRenderPass(renderPassInfo);
}

void VulkanEngine::createDescriptorSetLayout()
{
	//Set with only one uniform in vertex; Global
	vk::DescriptorSetLayoutBinding globalUboLayoutBinding{ 0, vk::DescriptorType::eUniformBuffer, 1,
														vk::ShaderStageFlagBits::eVertex, nullptr };

	vk::DescriptorSetLayoutCreateInfo globalLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), 1, &globalUboLayoutBinding };

	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(globalLayoutInfo));

	//Set with one uniform in vertex and one sampler in fragment; Local
	vk::DescriptorSetLayoutBinding modelUboLayoutBinding{ 0, vk::DescriptorType::eUniformBuffer, 1,
														vk::ShaderStageFlagBits::eVertex, nullptr };

	vk::DescriptorSetLayoutBinding samplerLayoutBinding{ 1, vk::DescriptorType::eCombinedImageSampler, 1,
														vk::ShaderStageFlagBits::eFragment, nullptr };

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { modelUboLayoutBinding, samplerLayoutBinding };
	vk::DescriptorSetLayoutCreateInfo modelLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), bindings.size(), bindings.data() };

	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(modelLayoutInfo));

	//Set with two uniforms in vertex; Global
	vk::DescriptorSetLayoutBinding globalLightLayoutBinding{ 1, vk::DescriptorType::eUniformBuffer, 1,
														vk::ShaderStageFlagBits::eVertex, nullptr };
	bindings[0] = globalUboLayoutBinding;
	bindings[1] = globalLightLayoutBinding;
	vk::DescriptorSetLayoutCreateInfo lightLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), bindings.size(), bindings.data() };
	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(lightLayoutInfo));

	//Set with one uniform in vertex and two samplers in fragment; Local
	vk::DescriptorSetLayoutBinding normalMapLayoutBinding{ 2, vk::DescriptorType::eCombinedImageSampler, 1,
														vk::ShaderStageFlagBits::eFragment, nullptr };
	std::array<vk::DescriptorSetLayoutBinding, 3> bindings3 = { modelUboLayoutBinding, samplerLayoutBinding, normalMapLayoutBinding };
	vk::DescriptorSetLayoutCreateInfo bumpLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), bindings3.size(), bindings3.data() };
	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(bumpLayoutInfo));
	//Set with three uniform in vertex; Global;
	vk::DescriptorSetLayoutBinding cameraLayoutBinding{ 2, vk::DescriptorType::eUniformBuffer, 1,
														vk::ShaderStageFlagBits::eVertex , nullptr };
	bindings3[0] = globalUboLayoutBinding;
	bindings3[1] = globalLightLayoutBinding;
	bindings3[2] = cameraLayoutBinding;
	vk::DescriptorSetLayoutCreateInfo cameraLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), bindings3.size(), bindings3.data() };
	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(cameraLayoutInfo));

	//Set with one uniform in vertex and three samplers in fragment; Local
	vk::DescriptorSetLayoutBinding depthMapLayoutBinding{ 3, vk::DescriptorType::eCombinedImageSampler, 1,
		vk::ShaderStageFlagBits::eFragment, nullptr };
	std::array<vk::DescriptorSetLayoutBinding, 4> bindings4 = { modelUboLayoutBinding, samplerLayoutBinding, normalMapLayoutBinding, depthMapLayoutBinding };
	vk::DescriptorSetLayoutCreateInfo parallaxLayoutInfo{ vk::DescriptorSetLayoutCreateFlags(), bindings4.size(), bindings4.data() };
	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(parallaxLayoutInfo));

	//EmptySet
	descriptorLayouts.push_back(logicDevice.createDescriptorSetLayout(vk::DescriptorSetLayoutCreateInfo{}));
}

void VulkanEngine::createGraphicsPipeline()
{
	if (graphicsPipelines.size() > 0)
	{
		for (auto& pipe : graphicsPipelines)
		{
			logicDevice.destroyPipeline(pipe.handle);
		}
		graphicsPipelines.clear();
		pipelineLayouts.clear();
	}
	graphicsPipelines.resize(8);

	Shader vertShader{ &logicDevice, "shaders/simpleV.spv", vk::ShaderStageFlagBits::eVertex, ShaderUsage::VS_PVTransform, ShaderUsage::VS_ModelTransform };
	Shader fragShader{ &logicDevice, "shaders/simpleTexturedF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture };

	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShader.getCreateInfo(), fragShader.getCreateInfo() };

	vk::VertexInputBindingDescription bindingDescription = Vertex3DT::bindingDescription();
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions = Vertex3DT::attributeDescriptions();
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{ vk::PipelineVertexInputStateCreateFlags(), 1, &bindingDescription,
															attributeDescriptions.size(), attributeDescriptions.data() };
	//Defines topology input to pipeline
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly{ vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList, VK_FALSE };

	vk::Viewport viewport{ 0.0f, 0.0f, static_cast<float>(swapExtent.width), static_cast<float>(swapExtent.height), 0.0f, 1.0f };

	vk::Rect2D scissor{ vk::Offset2D{0,0},swapExtent };

	vk::PipelineViewportStateCreateInfo viewportState{ vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scissor };

	vk::PipelineRasterizationStateCreateInfo rasterizer{ vk::PipelineRasterizationStateCreateFlags(), VK_FALSE, VK_FALSE, vk::PolygonMode::eFill,
											vk::CullModeFlagBits::eBack , vk::FrontFace::eCounterClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f };

	vk::PipelineMultisampleStateCreateInfo multisampling{ vk::PipelineMultisampleStateCreateFlags(), vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr,
											VK_FALSE, VK_FALSE };

	vk::PipelineDepthStencilStateCreateInfo depthStencil{ vk::PipelineDepthStencilStateCreateFlags(), VK_TRUE, VK_TRUE, vk::CompareOp::eLessOrEqual,
											VK_FALSE, VK_FALSE, vk::StencilOpState(), vk::StencilOpState(), 0.0f, 1.0f };

	vk::PipelineColorBlendAttachmentState colorBlendAttachment{ VK_TRUE, vk::BlendFactor::eSrcAlpha, vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd,
													vk::BlendFactor::eOne, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
													vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA };

	vk::PipelineColorBlendStateCreateInfo colorBlending{ vk::PipelineColorBlendStateCreateFlags(), VK_FALSE, vk::LogicOp::eCopy,
										1, &colorBlendAttachment, {{0.0f,0.0f,0.0f,0.0f}} };

	vk::DescriptorSetLayout descLayouts[2];
	pipelineLayouts.resize(5);
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo{ vk::PipelineLayoutCreateFlags(), 2, descLayouts,
													0, nullptr };
	//Create first pipeline layout
	descLayouts[0] = descriptorLayouts[0];
	descLayouts[1] = descriptorLayouts[1];
									//Descriptor sets corespond to first and second layout used in pipeline layout info respectively
	pipelineLayouts[0] = PipelineLayout{ &logicDevice, logicDevice.createPipelineLayout(pipelineLayoutInfo), &descriptorLayouts[0], &descriptorLayouts[1] };
	//Create second pipeline layout
	descLayouts[0] = descriptorLayouts[2];
	descLayouts[1] = descriptorLayouts[1];
									//Descriptor sets corespond to first and second layout used in pipeline layout info respectively
	pipelineLayouts[1] = PipelineLayout{ &logicDevice, logicDevice.createPipelineLayout(pipelineLayoutInfo), &descriptorLayouts[2], &descriptorLayouts[1] };
	//Create third pipeline layout
	descLayouts[0] = descriptorLayouts[4];
	descLayouts[1] = descriptorLayouts[3];
									//Descriptor sets corespond to first and second layout used in pipeline layout info respectively
	pipelineLayouts[2] = PipelineLayout{ &logicDevice, logicDevice.createPipelineLayout(pipelineLayoutInfo), &descriptorLayouts[4], &descriptorLayouts[3] };
	//Create third pipeline layout
	descLayouts[0] = descriptorLayouts[4];
	descLayouts[1] = descriptorLayouts[5];
	//Descriptor sets corespond to first and second layout used in pipeline layout info respectively
	pipelineLayouts[3] = PipelineLayout{ &logicDevice, logicDevice.createPipelineLayout(pipelineLayoutInfo), &descriptorLayouts[4], &descriptorLayouts[5] };
	//Create fourth pipeline layout
	descLayouts[0] = descriptorLayouts[6];
	descLayouts[1] = descriptorLayouts[1];
	//Descriptor sets corespond to first and second layout used in pipeline layout info respectively
	pipelineLayouts[4] = PipelineLayout{ &logicDevice, logicDevice.createPipelineLayout(pipelineLayoutInfo), &descriptorLayouts[6], &descriptorLayouts[1] };
	                                                                           
	vk::GraphicsPipelineCreateInfo pipelineInfo{ vk::PipelineCreateFlags(), 2, shaderStages, &vertexInputInfo, &inputAssembly,
										nullptr, &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending,
										nullptr, pipelineLayouts[0], renderPass, 0, vk::Pipeline(), -1 };


	graphicsPipelines[static_cast<int>(PipelineType::eNoLight)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eNoLight)].layout = &pipelineLayouts[0];
	graphicsPipelines[static_cast<int>(PipelineType::eNoLight)].globalReq = vertShader.getGlobalUsage() | fragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eNoLight)].localReq = vertShader.getLocalUsage() | fragShader.getLocalUsage();
	//Create skybox shader
	depthStencil.setDepthWriteEnable(VK_FALSE);
	graphicsPipelines[static_cast<int>(PipelineType::eSkybox)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eSkybox)].layout = &pipelineLayouts[0];
	graphicsPipelines[static_cast<int>(PipelineType::eSkybox)].globalReq = vertShader.getGlobalUsage() | fragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eSkybox)].localReq = vertShader.getLocalUsage() | fragShader.getLocalUsage();
	//Create wireframe shader
	depthStencil.setDepthWriteEnable(VK_TRUE);
	rasterizer.setPolygonMode(vk::PolygonMode::eLine);
	graphicsPipelines[static_cast<int>(PipelineType::eWireframe)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eWireframe)].layout = &pipelineLayouts[0];
	graphicsPipelines[static_cast<int>(PipelineType::eWireframe)].globalReq = vertShader.getGlobalUsage() | fragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eWireframe)].localReq = vertShader.getLocalUsage() | fragShader.getLocalUsage();
	//Create phong shader
	Shader lightVertShader{ &logicDevice, "shaders/lightV.spv", vk::ShaderStageFlagBits::eVertex, ShaderUsage::VS_PVTransform | ShaderUsage::VS_Light, ShaderUsage::VS_ModelTransform };
	Shader phongFragShader{ &logicDevice, "shaders/phongF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture };
	rasterizer.setPolygonMode(vk::PolygonMode::eFill);
	shaderStages[0] = lightVertShader.getCreateInfo();
	shaderStages[1] = phongFragShader.getCreateInfo();
	pipelineInfo.setLayout(pipelineLayouts[1]);

	graphicsPipelines[static_cast<int>(PipelineType::ePhong)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::ePhong)].layout = &pipelineLayouts[1];
	graphicsPipelines[static_cast<int>(PipelineType::ePhong)].globalReq = lightVertShader.getGlobalUsage() | phongFragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::ePhong)].localReq = lightVertShader.getLocalUsage() | phongFragShader.getLocalUsage();
	//Create Toon shader
	Shader toonFragShader{ &logicDevice, "shaders/toonF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture };

	shaderStages[1] = toonFragShader.getCreateInfo();
	graphicsPipelines[static_cast<int>(PipelineType::eToon)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eToon)].layout = &pipelineLayouts[1];
	graphicsPipelines[static_cast<int>(PipelineType::eToon)].globalReq = lightVertShader.getGlobalUsage() | toonFragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eToon)].localReq = lightVertShader.getLocalUsage() | toonFragShader.getLocalUsage();
	//Create Bump map shader
	Shader tangentVertShader{ &logicDevice, "shaders/tangentSpaceV.spv", vk::ShaderStageFlagBits::eVertex, ShaderUsage::VS_PVTransform | ShaderUsage::VS_Light | ShaderUsage::VS_CameraPos, ShaderUsage::VS_ModelTransform | ShaderUsage::VS_Tangents };
	Shader bumpFragShader{ &logicDevice, "shaders/bumpMapPhongF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture | ShaderUsage::FS_NormalMap };
	shaderStages[0] = tangentVertShader.getCreateInfo();
	shaderStages[1] = bumpFragShader.getCreateInfo();
	vk::VertexInputBindingDescription bumpBindingDescription = Vertex3DTT::bindingDescription();
	std::vector<vk::VertexInputAttributeDescription> bumpAttributeDescriptions = Vertex3DTT::attributeDescriptions();
	vertexInputInfo.setPVertexBindingDescriptions(&bumpBindingDescription);
	vertexInputInfo.setVertexAttributeDescriptionCount(bumpAttributeDescriptions.size());
	vertexInputInfo.setPVertexAttributeDescriptions(bumpAttributeDescriptions.data());
	pipelineInfo.setLayout(pipelineLayouts[2]);
	graphicsPipelines[static_cast<int>(PipelineType::eBumpMap)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eBumpMap)].layout = &pipelineLayouts[2];
	graphicsPipelines[static_cast<int>(PipelineType::eBumpMap)].globalReq = tangentVertShader.getGlobalUsage() | bumpFragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eBumpMap)].localReq = tangentVertShader.getLocalUsage() | bumpFragShader.getLocalUsage();
	//Create parallax map shader
	Shader parallaxFragShader{ &logicDevice, "shaders/parallaxPhongF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture | ShaderUsage::FS_NormalMap | ShaderUsage::FS_DepthMap };
	shaderStages[0] = tangentVertShader.getCreateInfo();
	shaderStages[1] = parallaxFragShader.getCreateInfo();
	pipelineInfo.setLayout(pipelineLayouts[3]);
	graphicsPipelines[static_cast<int>(PipelineType::eParallax)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eParallax)].layout = &pipelineLayouts[3];
	graphicsPipelines[static_cast<int>(PipelineType::eParallax)].globalReq = tangentVertShader.getGlobalUsage() | parallaxFragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eParallax)].localReq = tangentVertShader.getLocalUsage() | parallaxFragShader.getLocalUsage();

	//Create orthographic shader
	Shader orthoVertShader{ &logicDevice, "shaders/orthoTexturedV.spv", vk::ShaderStageFlagBits::eVertex, ShaderUsage::Empty, ShaderUsage::VS_ModelTransform };
	Shader orthoFragShader{ &logicDevice, "shaders/simpleTexturedF.spv", vk::ShaderStageFlagBits::eFragment, ShaderUsage::Empty, ShaderUsage::FS_Texture };

	shaderStages[0] = orthoVertShader.getCreateInfo();
	shaderStages[1] = orthoFragShader.getCreateInfo();
	vk::VertexInputBindingDescription orthoBindingDescription = Vertex2DT::bindingDescription();
	std::vector<vk::VertexInputAttributeDescription> orthoAttributeDescriptions = Vertex2DT::attributeDescriptions();
	vertexInputInfo.setPVertexBindingDescriptions(&orthoBindingDescription);
	vertexInputInfo.setVertexAttributeDescriptionCount(orthoAttributeDescriptions.size());
	vertexInputInfo.setPVertexAttributeDescriptions(orthoAttributeDescriptions.data());
	pipelineInfo.setLayout(pipelineLayouts[4]);

	graphicsPipelines[static_cast<int>(PipelineType::eOrthoTextured)].handle = logicDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);
	graphicsPipelines[static_cast<int>(PipelineType::eOrthoTextured)].layout = &pipelineLayouts[4];
	graphicsPipelines[static_cast<int>(PipelineType::eOrthoTextured)].globalReq = orthoVertShader.getGlobalUsage() | orthoFragShader.getGlobalUsage();
	graphicsPipelines[static_cast<int>(PipelineType::eOrthoTextured)].localReq = orthoVertShader.getLocalUsage() | orthoFragShader.getLocalUsage();
}

void VulkanEngine::createDescriptorPool()
{
	std::array<vk::DescriptorPoolSize, 2> poolSizes{ vk::DescriptorPoolSize{vk::DescriptorType::eUniformBuffer, 128},
														vk::DescriptorPoolSize{ vk::DescriptorType::eCombinedImageSampler, 127 } };
	vk::DescriptorPoolCreateInfo poolInfo{ vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, 128, poolSizes.size(), poolSizes.data() };

	descriptorPool = logicDevice.createDescriptorPool(poolInfo);
}

void VulkanEngine::recreateSwapChain()
{
	VulkanBase::recreateSwapChain();
	//NOTE if createCommandBuffer is not called every frame it needs to be called here as well
}

void VulkanEngine::draw()
{
	vk::ResultValue<uint32_t> imageIndex = logicDevice.acquireNextImageKHR(swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, vk::Fence());
	if (imageIndex.result == vk::Result::eErrorOutOfDateKHR)
	{
		recreateSwapChain();
		return;
	}
	else if (imageIndex.result != vk::Result::eSuccess && imageIndex.result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphore };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphore };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::SubmitInfo submit{ 1, waitSemaphores, waitStages,1, &commandBuffers[imageIndex.value], 1, signalSemaphores };

	queue.submit(submit, vk::Fence());
	queue.waitIdle();
	vk::SwapchainKHR swapChains[] = { swapChain };
	vk::PresentInfoKHR presentInfo{ 1, signalSemaphores, 1, swapChains, &imageIndex.value, nullptr };

	vk::Result result = queue.presentKHR(presentInfo);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		recreateSwapChain();
	}
	else if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("failed to present swap chain image!");
	}
}

Pipeline VulkanEngine::getPipeline(PipelineType pipeline)
{
	return graphicsPipelines[static_cast<int>(pipeline)];
}

Result VulkanEngine::transferObject(std::list<std::shared_ptr<GraphicsComponent>>& from, std::list<std::shared_ptr<GraphicsComponent>>& to, int objectId)
{
	for (std::list<std::shared_ptr<GraphicsComponent>>::const_iterator it = from.begin(); it != from.end(); it++)
	{
		if ((*it)->id == objectId)
		{
			to.splice(findInsertIterator(to, *it), from, it);
			return Result::eSuccess;
		}
	}
	return Result::eNotFound;
}

std::list<std::shared_ptr<GraphicsComponent>>::const_iterator VulkanEngine::findInsertIterator(const std::list<std::shared_ptr<GraphicsComponent>>& list, const std::shared_ptr<GraphicsComponent>& item) const
{
	std::list<std::shared_ptr<GraphicsComponent>>::const_iterator result;
	std::list<std::shared_ptr<GraphicsComponent>>::const_reverse_iterator rIt, rLayerBegin, rLayerEnd;
	rLayerBegin = std::find_if(list.crbegin(), list.crend(), [item](std::shared_ptr<GraphicsComponent> x) { return x->getLayer() <= item->getLayer(); });
	if (rLayerBegin == list.crend())
	{
		result = list.begin();
	}
	else if ((*rLayerBegin)->getLayer() < item->getLayer())
	{
		//base returns forward iterator to a element after the one pointed to by reverse iterator.
		//That's perfect because we want to put the new element behind the one we found.
		result = rLayerBegin.base();
	}
	else
	{
		rLayerEnd = std::find_if(rLayerBegin, list.crend(), [item](std::shared_ptr<GraphicsComponent> x) { return x->getLayer() < item->getLayer(); });
		rIt = std::find_if(rLayerBegin, rLayerEnd, [item](std::shared_ptr<GraphicsComponent> x) { return x->getDrawType() == item->getDrawType(); });
		if (rIt == rLayerEnd)
		{
			result = rLayerBegin.base();
		}
		else
		{
			//base returns forward iterator to a element after the one pointed to by reverse iterator.
			//That's perfect because we want to put the new element behind the one we found.
			result = rIt.base();
		}
	}
	return result;
}
