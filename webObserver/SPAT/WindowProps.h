#pragma once
#include <string>

class WindowProps final
{
public:
	std::string name;
	size_t width;
	size_t height;
	WindowProps(const std::string& windowName, size_t w = 1600, size_t h= 900):name(windowName), width(w), height(h)
	{}
};
