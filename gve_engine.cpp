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
			gveWindow.resetWindowResizedFlag();
			recreateSwapChain();
			drawFrame();
		});
	}

	GveEngine::~GveEngine()
	{
		vkDestroyPipelineLayout(gveDevice.device(), pipelineLayout, nullptr);
	}
	
	void GveEngine::run()
	{
		while(!gveWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(gveDevice.device());
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

		if (vkCreatePipelineLayout(gveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	void GveEngine::createCommandBuffers()
	{
		commandBuffers.resize(gveSwapChain->imageCount());
		
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = gveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if(vkAllocateCommandBuffers(gveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void GveEngine::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			gveDevice.device(), 
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

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = gveSwapChain->getRenderPass();
		renderPassInfo.framebuffer = gveSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0,  0 };
		renderPassInfo.renderArea.extent = gveSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(gveSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(gveSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, gveSwapChain->getSwapChainExtent() };
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

		vkDeviceWaitIdle(gveDevice.device());

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