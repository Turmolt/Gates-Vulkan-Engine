#include "gve_window.hpp"

#include <iostream>

#include "gve_image.h"


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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);

		GLFWimage icons[1];
		GveImage::loadImage(icons[0], "resources/logo.png");
		glfwSetWindowIcon(window, 1, icons);
		GveImage::freeImage(icons[0]);

		glfwSetWindowSizeLimits(window, 400, 400, GLFW_DONT_CARE, GLFW_DONT_CARE);
			
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
		glfwSetWindowIconifyCallback(window, windowIconifiedCallback);
	}

	void GveWindow::framebufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto gveWindow = reinterpret_cast<GveWindow*>(glfwGetWindowUserPointer(window));
		gveWindow->framebufferResized = true;
		gveWindow->width = width;
		gveWindow->height = height;
		
		if(width > 0 && height > 0)
			gveWindow->invokeWindowResizedEvent();
	}

	void GveWindow::windowIconifiedCallback(GLFWwindow* window, int iconified)
	{
		auto gveWindow = reinterpret_cast<GveWindow*>(glfwGetWindowUserPointer(window));
		gveWindow->iconified = iconified;
	}

	void GveWindow::subscribeToWindowResized(std::function<void()> subscriber)
	{
		onWindowResizedSubscribers.push_back(subscriber);
	}

	void GveWindow::invokeWindowResizedEvent()
	{
		for (auto subscriber : onWindowResizedSubscribers)
		{
			subscriber();
		}
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
