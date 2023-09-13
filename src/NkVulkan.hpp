#ifndef NkVulkan_h
#define NkVulkan_h
#include"NkCommon.hpp"
namespace nk{
class VulkanContext{
public:

    // --- omitted ---
    VkInstance _instance; // Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
	VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface; // Vulkan window surface
    VkSwapchainKHR _swapchain; // from other articles
	VkFormat _swapchainImageFormat;// image format expected by the windowing system
	std::vector<VkImage> _swapchainImages;//array of images from the swapchain
	std::vector<VkImageView> _swapchainImageViews;//array of image-views from the swapchain

private:
    void init();
	void init_vulkan(); 
    void init_swapchain(); 
    void cleanup();

    void createInstance();
    
};
}
#endif