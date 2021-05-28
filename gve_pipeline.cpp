#include "gve_pipeline.hpp"

//std
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace gve
{
	GvePipeline::GvePipeline(
		GveDevice& device,
		const std::string& vertFilePath,
		const std::string& fragFilePath,
		const PipelineConfigInfo& configInfo) : gveDevice(device)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
	}
	
	std::vector<char> GvePipeline::readFile(const std::string& filepath)
	{
		std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

		if(!file.is_open())
		{
			throw std::runtime_error("failed to open file: " + filepath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
	
	void GvePipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(gveDevice.getDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}
		
	}
	
	void GvePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		createShaderModule(vertCode, &vertShaderModule);
		createShaderModule(fragCode, &fragShaderModule);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		//global variable setup for me shaders
		vertShaderStageInfo.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
		
		vkDestroyShaderModule(gveDevice.getDevice(), fragShaderModule, nullptr);
		vkDestroyShaderModule(gveDevice.getDevice(), vertShaderModule, nullptr);
	}

	
}