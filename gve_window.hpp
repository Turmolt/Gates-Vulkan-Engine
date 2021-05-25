#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include <GLFW/glfw3.h>

namespace gve
{

	class GveWindow
	{

	public:
		GveWindow(int w, int h, std::string name);
		~GveWindow();

		GveWindow(const GveWindow&) = delete;
		GveWindow& operator=(const GveWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
	
	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow *window;		
	};
	
}