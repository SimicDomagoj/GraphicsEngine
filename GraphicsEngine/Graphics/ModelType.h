#pragma once
/**
	Enumerator used for defining load type.
*/
enum class ModelType
{
	e3D = 0,		//*< 3D model with vertices, uvs and normals.
	e2D,			//*< 2D model with vertices and uvs.
	e3DTangent		//*< 3D model with vertices, uvs, normals and tangents.
};