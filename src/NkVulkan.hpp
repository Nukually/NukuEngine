#ifndef NkVulkan_h
#define NkVulkan_h
#include"NkCommon.hpp"
#include"NkWindow.hpp"
namespace nk{
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
class VulkanContext{
private:
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	#ifdef NDEBUG
	const bool enableValidationLayers = false;
	#else
	const bool enableValidationLayers = true;
	#endif
private:
    // --- omitted ---
	Window &window;
    VkInstance _instance; // Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
	VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkQueue _graphicsQueue;
	VkQueue _presentQueue;
	VkSurfaceKHR _surface; // Vulkan window surface
    VkSwapchainKHR _swapchain; // from other articles
	VkFormat _swapchainImageFormat;// image format expected by the windowing system
	std::vector<VkImage> _swapchainImages;//array of images from the swapchain
	std::vector<VkImageView> _swapchainImageViews;//array of image-views from the swapchain

public:
	void init();
	void cleanup();
	VulkanContext(Window &window);

private:
	void init_vulkan(); 
    void init_swapchain(); 
    void createInstance();
	void setupDebugMessenger();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanContext::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	
};
}
#endif