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

	
}