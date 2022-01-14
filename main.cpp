#include "gve_engine.hpp"

//std
#include <cstdlib>
#include <iostream>

int main()
{
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