#include "gve_window.hpp"

namespace gve
{

	GveWindow::GveWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name }
	{
		initWindow();
	}

	GveWindow::~GveWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void GveWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);;
	}


	void GveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void GveWindow::getFrameBufferSize(int& height, int& width)
	{
		glfwGetFramebufferSize(window, &width, &height);
	}

	
}