#pragma once

/**
	Enumerator describing shader usage.
	Defines flags which represent different uniform variables shader could contain.
*/
enum class ShaderUsage
{
	Empty = 0,				//*< Shader has no variables.
	VS_PVTransform = 1,		//*< Shader has projection-view transformation.
	VS_Light = 2,			//*< Shader has lights.
	VS_ModelTransform = 4,	//*< Shader has model transformation.
	VS_Tangents = 8,		//*< Shader has tangent and bitangent information.
	VS_CameraPos = 16,		//*< Shader has information about camera position.
	FS_Texture = 32,		//*< Shader has texture.
	FS_NormalMap = 64,		//*< Shader has a normal map.
	FS_DepthMap = 128		//*< Shader has a depth map.
};

/**
	Logic or operator.
	@param a left operator.
	@param b right operator.
	@return result of OR operation between operators.
*/
inline ShaderUsage operator|(ShaderUsage a, ShaderUsage b)
{
	return static_cast<ShaderUsage>(static_cast<int>(a) | static_cast<int>(b));
}