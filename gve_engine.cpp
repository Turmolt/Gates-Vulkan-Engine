#include "gve_engine.hpp"
#include <stdexcept>
#include <array>

namespace gve {
	GveEngine::GveEngine()
	{
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
		gveWindow.subscribeToWindowResized([this]() {
			glfwWaitEvents();
			gveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			drawFrame();
		});
		initImgui();
	}

	GveEngine::~GveEngine()
	{
		vkDestroyDescriptorPool(gveDevice.getDevice(), imguiPool, nullptr);
		ImGui_ImplVulkan_Shutdown();
		vkDestroyPipelineLayout(gveDevice.getDevice(), pipelineLayout, nullptr);
	}
	
	void GveEngine::run()
	{
		while(!gveWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(gveDevice.getDevice());
	}

	void GveEngine::createPipeline()
	{
		PipelineConfigInfo pipelineConfig{};
		GvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = gveSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		gvePipeline = std::make_unique<GvePipeline>(
			gveDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			pipelineConfig);
	}

	void GveEngine::createPipelineLayout()
	{

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(gveDevice.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void GveEngine::createCommandBuffers()
	{
		commandBuffers.resize(gveSwapChain->imageCount());
		
		VkCommandBufferAllocateInfo allocInfo = vkinit::command_buffer_allocate_info(gveDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		if(vkAllocateCommandBuffers(gveDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void GveEngine::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			gveDevice.getDevice(), 
			gveDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	void GveEngine::recordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo = 
			vkinit::render_pass_begin_info(
				gveSwapChain->getRenderPass(),
				gveSwapChain->getFrameBuffer(imageIndex),
				gveSwapChain->getSwapChainExtent()
			);

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;

		auto extent = gveSwapChain->getSwapChainExtent();
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, extent };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		gvePipeline->bind(commandBuffers[imageIndex]);
		vkCmdDraw(commandBuffers[imageIndex], 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}


	void GveEngine::initImgui()
	{
		//1: create descriptor pool for IMGUI
		// the size of the pool is very oversize, but it's copied from imgui demo itself.
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		
		vkCreateDescriptorPool(gveDevice.getDevice(), &pool_info, nullptr, &imguiPool);


		// 2: initialize imgui library

		//this initializes the core structures of imgui
		ImGui::CreateContext();

		//this initializes imgui for SDL
		ImGui_ImplGlfw_InitForVulkan(gveWindow.getWindow(), false);

		//this initializes imgui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = gveDevice.getInstance();
		init_info.PhysicalDevice = gveDevice.getPhysicalDevice();
		init_info.Device = gveDevice.getDevice();
		init_info.Queue = gveDevice.getGraphicsQueue();
		init_info.DescriptorPool = imguiPool;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		ImGui_ImplVulkan_Init(&init_info, gveSwapChain->getRenderPass());
	}



	/// <summary>
	/// Draws the current frame
	/// </summary>
	void GveEngine::drawFrame()
	{
		uint32_t imageIndex;
		auto result = gveSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		} 
		if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);

		//ImGui_ImplVulkan_NewFrame();
		//ImGui_ImplGlfw_NewFrame();

		//ImGui::NewFrame();

		//ImGui::ShowDemoWindow();

		result = gveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || gveWindow.wasWindowResized())
		{
			gveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if(result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

	}


	void GveEngine::recreateSwapChain()
	{
		auto extent = gveWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = gveWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(gveDevice.getDevice());

		if (gveSwapChain == nullptr) 
		{
			gveSwapChain = std::make_unique<GveSwapChain>(gveDevice, extent);
		}
		else 
		{
			gveSwapChain = std::make_unique<GveSwapChain>(gveDevice, extent, std::move(gveSwapChain));
			if (gveSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();
	}
}