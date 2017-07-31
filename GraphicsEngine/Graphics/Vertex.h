#pragma once
#include <vulkan\vulkan.hpp>
#include <glm\glm.hpp>
#include <vector>
#include <glm/gtx/hash.hpp>

struct Vertex3DT
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;

	bool operator==(const Vertex3DT& other) const
	{
		return pos == other.pos && normal == other.normal && uv == other.uv;
	}
	static vk::VertexInputBindingDescription bindingDescription()
	{
		return vk::VertexInputBindingDescription{ 0, sizeof(Vertex3DT), vk::VertexInputRate::eVertex };
	}
	static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions()
	{
		return std::vector<vk::VertexInputAttributeDescription>{vk::VertexInputAttributeDescription{ 0,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DT, pos) },
														vk::VertexInputAttributeDescription{ 1,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DT, normal)},
														vk::VertexInputAttributeDescription{ 2,0,vk::Format::eR32G32Sfloat, offsetof(Vertex3DT, uv)} };
	}
};

struct Vertex3DTT
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
	bool operator==(const Vertex3DTT& other) const
	{
		return pos == other.pos && normal == other.normal && uv == other.uv && tangent == other.tangent && bitangent == other.bitangent;
	}
	static vk::VertexInputBindingDescription bindingDescription()
	{
		return vk::VertexInputBindingDescription{ 0, sizeof(Vertex3DTT), vk::VertexInputRate::eVertex };
	}
	static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions()
	{
		return std::vector<vk::VertexInputAttributeDescription>{vk::VertexInputAttributeDescription{ 0,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DTT, pos) },
			vk::VertexInputAttributeDescription{ 1,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DTT, normal) },
			vk::VertexInputAttributeDescription{ 2,0,vk::Format::eR32G32Sfloat, offsetof(Vertex3DTT, uv) },
			vk::VertexInputAttributeDescription{ 3,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DTT, tangent) },
			vk::VertexInputAttributeDescription{ 4,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex3DTT, bitangent) } };
	}
};

struct Vertex2DT
{
	glm::vec2 pos;
	glm::vec2 texCoord;
	bool operator==(const Vertex2DT& other) const
	{
		return pos == other.pos && texCoord == other.texCoord;
	}
	static vk::VertexInputBindingDescription bindingDescription()
	{
		return vk::VertexInputBindingDescription{ 0, sizeof(Vertex2DT), vk::VertexInputRate::eVertex };
	}
	static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions()
	{
		return std::vector<vk::VertexInputAttributeDescription>{vk::VertexInputAttributeDescription{ 0,0,vk::Format::eR32G32Sfloat, offsetof(Vertex2DT, pos) },
			vk::VertexInputAttributeDescription{ 1,0,vk::Format::eR32G32Sfloat, offsetof(Vertex2DT, texCoord) } };
	}
};

struct Vertex2DC
{
	glm::vec2 pos;
	glm::vec3 color;

	static vk::VertexInputBindingDescription bindingDescription()
	{
		return vk::VertexInputBindingDescription{ 0, sizeof(Vertex2DC), vk::VertexInputRate::eVertex };
	}
	static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions()
	{
		return std::vector<vk::VertexInputAttributeDescription>{vk::VertexInputAttributeDescription{ 0,0,vk::Format::eR32G32Sfloat, offsetof(Vertex2DC, pos) },
			vk::VertexInputAttributeDescription{ 1,0,vk::Format::eR32G32B32Sfloat, offsetof(Vertex2DC, color) } };
	}
};

namespace std
{
	template<> struct hash<Vertex2DT>
	{
		size_t operator()(Vertex2DT const& vertex) const
		{
			return (hash<glm::vec2>()(vertex.pos) ^	(hash<glm::vec2>()(vertex.texCoord) << 1));
		}
	};

	template<> struct hash<Vertex3DT>
	{
		size_t operator()(Vertex3DT const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.uv) << 1);
		}
	};

	template<> struct hash<Vertex3DTT>
	{
		size_t operator()(Vertex3DTT const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.uv) << 1);
		}
	};
}