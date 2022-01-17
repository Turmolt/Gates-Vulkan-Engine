#pragma once
#include "gve_window.hpp"
#include "gve_pipeline.hpp"
#include "gve_device.hpp"
#include "gve_swap_chain.hpp"
#include "gve_vk_initializers.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <memory>
#include <vector>
#include <assert.h>



#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)


namespace gve
{
	class GveEngine
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		GveEngine();
		~GveEngine();

		GveEngine(const GveEngine&) = delete;
		GveEngine& operator=(const GveEngine&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void initImgui();
		
		GveWindow gveWindow{ WIDTH, HEIGHT, "Gates Vulkan Engine" };
		GveDevice gveDevice{ gveWindow };
		std::unique_ptr<GveSwapChain> gveSwapChain;
		std::unique_ptr<GvePipeline> gvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		VkDescriptorPool imguiPool;
	};
}
