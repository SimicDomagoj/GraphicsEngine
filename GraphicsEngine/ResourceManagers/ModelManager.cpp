#define TINYOBJLOADER_IMPLEMENTATION
#include<tiny_obj_loader.h>
#include "ModelManager.h"
#include"..\DebugTools\Assert.h"
#include "..\Core\GraphicsEngine.h"
#include<unordered_map>
#include"..\Graphics\Vertex.h"
#ifdef _DEBUG
#include<iostream>
#endif

ModelManager::ModelManager(const GraphicsEngine * engine)
{
	this->engine = engine;
}

std::shared_ptr<VModel> ModelManager::get(const std::string& filename, const ModelType& type)
{
	std::string name = extractName(filename);
	std::shared_ptr<VModel> resource;
	std::shared_ptr<VModel> (ModelManager::*loader)(const std::string& filename);	// Used to store pointer to a member function used to load the model.

	//Find full name depending on model type
	//Also determine which function is used to load the model if it is not found
	// so we don't have to go through switch statement again
	switch (type)
	{
	case ModelType::e3D:
		loader = &ModelManager::load3D;
		name += "3D";
		break;
	case ModelType::e3DTangent:
		loader = &ModelManager::load3DWithTangent;
		name += "3DT";
		break;
	case ModelType::e2D:
		loader = &ModelManager::load2D;
		name += "2D";
		break;
	default:
		ASSERT(false) // If this triggers there is case missed;
			break;
	}
	//Find a resource and return it if found.
	resource = find(name);
	if (resource != nullptr)
	{
#ifdef _DEBUG
		hit++;
#endif
		return resource;
	}
#ifdef _DEBUG
	miss++;
#endif
	//load and return resource.
	return (this->*loader)(filename);
}

ModelManager::~ModelManager()
{
#ifdef _DEBUG
	std::cout << "Model hit: " << hit << std::endl;
	std::cout << "Model miss: " << miss << std::endl;
#endif
}

std::shared_ptr<VModel> ModelManager::load3D(const std::string& filename)
{
	std::string name = extractName(filename) + "3D";
	std::shared_ptr<VModel> model = std::make_shared<VModel>();
	std::unordered_map<Vertex3DT, int> uniqueVertices = {};
	std::vector<Vertex3DT> vertices;
	std::vector<uint32_t> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	// Load the model
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str()))
	{
		throw std::runtime_error(err);
	}
	// Fill vertices, uv coordinates and normals
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex3DT vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			// Origin of texture coordinates in Vulkan is the top-left corner,
			// whereas the OBJ format assumes the bottom-left corner
			// so we need to correct it by subtracting y coordinate from 1.0
			vertex.uv = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			if (attrib.normals.size() > 0)
			{
				vertex.normal = { attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2] };
			}
			else
			{
				vertex.normal = { 0,0,1 };
			}

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = vertices.size();
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	// Create models vertex and index buffer
	model->vertices = engine->createVertexBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size());
	model->indices = engine->createIndexBuffer(indices);
	// Add model to collection and return it.
	add(name, model);
	return model;
}

std::shared_ptr<VModel> ModelManager::load3DWithTangent(const std::string& filename)
{
	std::string name = extractName(filename) + "3DT";
	std::shared_ptr<VModel> model = std::make_shared<VModel>();
	std::unordered_map<Vertex3DTT, int> uniqueVertices = {};
	std::vector<Vertex3DTT> vertices;
	std::vector<uint32_t> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	// Load the model
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str()))
	{
		throw std::runtime_error(err);
	}
	// Fill vertices, uv coordinates and normals
	for (const auto& shape : shapes)
	{
		for (unsigned int i = 0; i < shape.mesh.indices.size(); i += 3)
		{
			tinyobj::index_t i1 = shape.mesh.indices[i], i2 = shape.mesh.indices[i + 1], i3 = shape.mesh.indices[i + 2];
			Vertex3DTT v1, v2, v3;
			v1.pos = { attrib.vertices[3 * i1.vertex_index + 0],	attrib.vertices[3 * i1.vertex_index + 1], attrib.vertices[3 * i1.vertex_index + 2] };
			v2.pos = { attrib.vertices[3 * i2.vertex_index + 0],	attrib.vertices[3 * i2.vertex_index + 1], attrib.vertices[3 * i2.vertex_index + 2] };
			v3.pos = { attrib.vertices[3 * i3.vertex_index + 0],	attrib.vertices[3 * i3.vertex_index + 1], attrib.vertices[3 * i3.vertex_index + 2] };

			// Origin of texture coordinates in Vulkan is the top-left corner,
			// whereas the OBJ format assumes the bottom-left corner
			// so we need to correct it by subtracting y coordinate from 1.0
			v1.uv = { attrib.texcoords[2 * i1.texcoord_index + 0],	1.f - attrib.texcoords[2 * i1.texcoord_index + 1] };
			v2.uv = { attrib.texcoords[2 * i2.texcoord_index + 0],	1.f - attrib.texcoords[2 * i2.texcoord_index + 1] };
			v3.uv = { attrib.texcoords[2 * i3.texcoord_index + 0],	1.f - attrib.texcoords[2 * i3.texcoord_index + 1] };

			v1.normal = { attrib.normals[3 * i1.normal_index + 0],	attrib.normals[3 * i1.normal_index + 1], attrib.normals[3 * i1.normal_index + 2] };
			v2.normal = { attrib.normals[3 * i2.normal_index + 0],	attrib.normals[3 * i2.normal_index + 1], attrib.normals[3 * i2.normal_index + 2] };
			v3.normal = { attrib.normals[3 * i3.normal_index + 0],	attrib.normals[3 * i3.normal_index + 1], attrib.normals[3 * i3.normal_index + 2] };

			//calculate tangents and bitangents
			glm::vec3 dV1 = v2.pos - v1.pos;
			glm::vec3 dV2 = v3.pos - v1.pos;

			glm::vec2 dUV1 = v2.uv - v1.uv;
			glm::vec2 dUV2 = v3.uv - v1.uv;
			float x = 1.f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

			glm::vec3 tangent = (dV1 * dUV2.y - dV2 * dUV1.y) * x;
			glm::vec3 bitangent = (dV2 * dUV1.x - dV1 * dUV2.x) * x;
			//gram-schmidt ortogonalization
			tangent = glm::normalize(tangent - v1.normal * glm::dot(v1.normal, tangent));
			/*optional if we are using symetric models and textures
			if (glm::dot(glm::cross(n, t), b) < 0.0f)
			{
				t = t * -1.0f;
			}*/

			v1.tangent = tangent;
			v2.tangent = tangent;
			v3.tangent = tangent;
			v1.bitangent = bitangent;
			v2.bitangent = bitangent;
			v3.bitangent = bitangent;

			if (uniqueVertices.count(v1) == 0)
			{
				uniqueVertices[v1] = vertices.size();
				vertices.push_back(v1);
			}
			else
			{
				vertices[uniqueVertices[v1]].tangent += v1.tangent;
				vertices[uniqueVertices[v1]].bitangent += v1.bitangent;
			}
			indices.push_back(uniqueVertices[v1]);

			if (uniqueVertices.count(v2) == 0)
			{
				uniqueVertices[v2] = vertices.size();
				vertices.push_back(v2);
			}
			else
			{
				vertices[uniqueVertices[v2]].tangent += v2.tangent;
				vertices[uniqueVertices[v2]].bitangent += v2.bitangent;
			}
			indices.push_back(uniqueVertices[v2]);

			if (uniqueVertices.count(v3) == 0)
			{
				uniqueVertices[v3] = vertices.size();
				vertices.push_back(v3);
			}
			else
			{
				vertices[uniqueVertices[v3]].tangent += v3.tangent;
				vertices[uniqueVertices[v3]].bitangent += v3.bitangent;
			}
			indices.push_back(uniqueVertices[v3]);
		}
	}
	// Create models vertex and index buffer
	model->vertices = engine->createVertexBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size());
	model->indices = engine->createIndexBuffer(indices);
	// Add model to collection and return it.
	add(name, model);
	return model;
}

std::shared_ptr<VModel> ModelManager::load2D(const std::string& filename)
{
	std::string name = extractName(filename) + "2D";
	std::shared_ptr<VModel> model = std::make_shared<VModel>();
	std::unordered_map<Vertex2DT, int> uniqueVertices = {};
	std::vector<Vertex2DT> vertices;
	std::vector<uint32_t> indices;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	// Load the model
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str()))
	{
		throw std::runtime_error(err);
	}
	// Fill vertices and uv coordinates
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex2DT vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1]
			};

			// Origin of texture coordinates in Vulkan is the top-left corner,
			// whereas the OBJ format assumes the bottom-left corner
			// so we need to correct it by subtracting y coordinate from 1.0
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = vertices.size();
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
	// Create models vertex and index buffer
	model->vertices = engine->createVertexBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size());
	model->indices = engine->createIndexBuffer(indices);
	// Add model to collection and return it.
	add(name, model);
	return model;
}