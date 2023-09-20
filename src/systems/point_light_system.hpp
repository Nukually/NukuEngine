#pragma once

#include "NkCamera.hpp"
#include "NkVulkan.hpp"
#include "NkFrameinfo.hpp"
#include "NkGameobject.hpp"
#include "NkPipeline.hpp"


namespace nk {
class PointLightSystem {
 public:
  PointLightSystem(
      VulkanContext &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo &frameInfo, GlobalUbo &ubo);
  void render(FrameInfo &frameInfo);

 private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  VulkanContext &device;

  std::unique_ptr<NkPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};
}  // namespace nk
