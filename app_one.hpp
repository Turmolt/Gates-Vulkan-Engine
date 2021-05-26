#pragma once
#include "gve_window.hpp"
#include "gve_pipeline.hpp"
#include "gve_device.hpp"

namespace gve
{
	class AppOne
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		GveWindow gveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		GvePipeline gvePipeline{ "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv" };
		GveDevice gveDevice{ gveWindow };
	};


}
