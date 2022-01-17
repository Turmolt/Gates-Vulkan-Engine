#pragma once

#define GLFW_INCLUDE_VULKAN
#include <stdexcept>
#include <string>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

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
		bool wasWindowResized() { return framebufferResized; }
		bool isIconified() { return iconified; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		void subscribeToWindowResized(std::function<void()> subscriber);
		
		GLFWwindow* getWindow() { return window; }

	private:

		static void framebufferResizedCallback(GLFWwindow* window, int width, int height);
		static void windowIconifiedCallback(GLFWwindow* window, int iconified);
		void initWindow();

		int width;
		int height;
		bool framebufferResized = false;
		bool iconified = false;

		void invokeWindowResizedEvent();
		std::vector<std::function<void()>> onWindowResizedSubscribers;

		std::string windowName;
		GLFWwindow *window;		
	};
	
}