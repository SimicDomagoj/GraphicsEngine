#include "VModel.h"

VModel::VModel(){}

VModel::VModel(VModel && x) : vertices{ std::move(x.vertices) }, indices{ std::move(x.indices) } {}

VModel & VModel::operator=(VModel && x)
{
	if (this != &x)
	{
		vertices = std::move(x.vertices);
		indices = std::move(x.indices);
	}
	return *this;
}
