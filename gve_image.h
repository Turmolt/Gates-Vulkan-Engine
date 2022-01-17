#pragma once
#include <GLFW/glfw3.h>

class GveImage
{
public:
	static void loadImage(GLFWimage& image, const char* path);
	static void freeImage(const GLFWimage& image);
};

