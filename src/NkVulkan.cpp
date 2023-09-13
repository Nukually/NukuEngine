#include"NkVulkan.hpp"
namespace nk{
VulkanContext::VulkanContext()
{
    init();
}
void VulkanContext::init()
{
    init_vulkan();
    init_swapchain();
}
void VulkanContext::init_vulkan()
{
    //init vulkan
    createInstance();
}
void VulkanContext::init_swapchain()
{
    //init swapchain
}
void VulkanContext::cleanup()
{
    //clean up
    vkDestroyInstance(_instance, nullptr);
}
VulkanContext::~VulkanContext()
{
    //析构
    cleanup();
}
void VulkanContext::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    if (vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}
}