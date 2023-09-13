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
}
void VulkanContext::init_swapchain()
{
    //init swapchain
}
void VulkanContext::cleanup()
{
    //clean up
}
VulkanContext::~VulkanContext()
{
    cleanup();//析构
}
}