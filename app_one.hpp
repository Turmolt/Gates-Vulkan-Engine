#pragma once
#include "gve_window.hpp"
#include "gve_pipeline.hpp"
#include "gve_device.hpp"
#include "gve_swap_chain.hpp"

#include <memory>
#include <vector>

namespace gve
{
	class AppOne
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		AppOne();
		~AppOne();

		AppOne(const AppOne&) = delete;
		AppOne& operator=(const AppOne&) = delete;

		void run();
	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
		
		GveWindow gveWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		GveDevice gveDevice{ gveWindow };
		GveSwapChain gveSwapChain{ gveDevice, gveWindow.getExtent() };
		std::unique_ptr<GvePipeline> gvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}
