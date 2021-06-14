#pragma once

#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
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
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		void getFrameBufferSize(int& height, int& width);
	
	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow *window;		
	};
	
}