#pragma once

/**
	Pipeline type enumerator.
	Enumerator which represents differenet types of a shader. 
*/
enum class PipelineType { 
	eNoLight = 0,	//*< Simple shader.
	eOrthoTextured,	//*< OrthoGraphic 2D shader
	ePhong,			//*< Shader using Phong lighting model.
	eToon,			//*< Shader using Toon lighting model.
	eWireframe,		//*< Shader for drawing wireframe version of objects.
	eSkybox,		//*< Shader used to draw a skybox.
	eBumpMap,		//*< Shader used for bump mapping.
	eParallax		//*< Shader used for parallax mapping.
};