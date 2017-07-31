#pragma once
#include<exception>

class GraphicsException : public virtual std::exception
{
public:
	GraphicsException() {}
	virtual char const * what() const { return "Object does not have graphics!"; }
	virtual ~GraphicsException() {}
};