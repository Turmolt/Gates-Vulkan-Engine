#include "gve_vk_initializers.h"

VkCommandPoolCreateInfo vkinit::command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = flags;
	return poolInfo;
}

VkCommandBufferAllocateInfo vkinit::command_buffer_allocate_info(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = level;
	allocInfo.commandPool = pool;
	allocInfo.commandBufferCount = static_cast<uint32_t>(count);
	return allocInfo;
}

VkRenderPassBeginInfo vkinit::render_pass_begin_info(VkRenderPass renderpass, VkFramebuffer framebuffer, VkExtent2D extent) 
{
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderpass;
	renderPassInfo.framebuffer = framebuffer;

	renderPassInfo.renderArea.offset = { 0,  0 };
	renderPassInfo.renderArea.extent = extent;

	return renderPassInfo;
}