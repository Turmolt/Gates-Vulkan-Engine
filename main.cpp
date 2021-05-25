#include "app_one.hpp"

//std
#include <cstdlib>
#include <iostream>

int main()
{
	gve::AppOne app{  };

	try
	{
		app.run();
	}catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}