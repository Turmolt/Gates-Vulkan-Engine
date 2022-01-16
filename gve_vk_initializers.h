#pragma once
#include <vulkan/vulkan_core.h>
#include <memory>
#include <vector>

namespace vkinit {
	VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
	VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	VkRenderPassBeginInfo render_pass_begin_info(VkRenderPass renderpass, VkFramebuffer framebuffer, VkExtent2D extent);
}