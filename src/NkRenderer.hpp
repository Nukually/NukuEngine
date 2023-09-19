#pragma once

#include "NkVulkan.hpp"
#include "NkSwapchain.hpp"
#include "NkWindow.hpp"

namespace nk {
class NkRenderer {
 public:
  NkRenderer(NkWindow &window, VulkanContext &device);
  ~NkRenderer();

  NkRenderer(const NkRenderer &) = delete;
  NkRenderer &operator=(const NkRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const { return _NkSwapChain->getRenderPass(); }
  float getAspectRatio() const { return _NkSwapChain->extentAspectRatio(); }
  bool isFrameInProgress() const { return isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted && "Cannot get frame index when frame not in progress");
    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

 private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  NkWindow &_window;
  VulkanContext &_device;
  std::unique_ptr<NkSwapChain> _swapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex{0};
  bool isFrameStarted{false};
};
}  // namespace nk
