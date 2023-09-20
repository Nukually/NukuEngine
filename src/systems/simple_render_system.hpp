#pragma once

#include "NkCamera.hpp"
#include "NkVulkan.hpp"
#include "NkFrameinfo.hpp"
#include "NkGameobject.hpp"
#include "NkPipeline.hpp"


namespace nk {
class SimpleRenderSystem {
 public:
  SimpleRenderSystem(
      VulkanContext &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  VulkanContext &device;

  std::unique_ptr<NkPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace nk
