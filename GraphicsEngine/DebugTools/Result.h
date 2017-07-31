#pragma once

enum class Result
{
	eSuccess = 0,		//*< Operation was succesfull.
	eNotFound,			//*< Object on which the operation was to be performed, was not found.
	eAssignedElsewhere	//*< Object is already assigned elsewhere.
};