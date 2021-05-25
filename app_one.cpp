#include "app_one.hpp"

namespace gve {
	void AppOne::run()
	{
		while(!gveWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}