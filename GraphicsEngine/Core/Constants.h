#pragma once
#include"glm\glm.hpp"

/**
	Namespace containing various constant.
*/
namespace cnst
{
	const glm::vec4 upVector{ 0.0, 1.0, 0.0, 0.0 };			//*< Coordinate system's up vector.
	const glm::vec4 forwardVector{ 0.0, 0.0, 1.0, 0.0 };	//*< Coordinate system's forward vector.
	const glm::vec4 rightVector{ 1.0, 0.0, 0.0, 0.0 };		//*< Coordinate system's right vector.
}