#pragma once
#include <string>
#include <vector>
#include "gve_device.hpp"

namespace gve {

	struct PipelineConfigInfo {};
	
	class GvePipeline
	{
	public:
		GvePipeline(
			GveDevice& device, 
			const std::string& vertFilePath, 
			const std::string& fragFilePath, 
			const PipelineConfigInfo& configInfo);
		~GvePipeline();

		GvePipeline(const GvePipeline&) = delete;
		void operator=(const GvePipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
	
	private:
		static std::vector<char> readFile(const std::string& filepath);

		GveDevice &gveDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;

		void createRenderPass();

		void createGraphicsPipeline(
			const std::string& vertFilePath,
			const std::string& fragFilePath,
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	};
}
 
 