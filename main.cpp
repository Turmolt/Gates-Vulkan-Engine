#include "gve_engine.hpp"

//std
#include <cstdlib>
#include <iostream>
#include <Windows.h>

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	gve::GveEngine gveEngine{  };

	try
	{
		gveEngine.run();
	}catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}