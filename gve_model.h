#pragma once

#include "gve_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_TO_ONE
#include <glm.hpp>
#include <vector>

namespace gve {
	class GveModel
	{
	public:
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		GveModel(GveDevice& device, const std::vector<Vertex>& vertices);
		~GveModel();

		GveModel(const GveModel&) = delete;
		GveModel& operator=(const GveModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);

		GveDevice &gveDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}