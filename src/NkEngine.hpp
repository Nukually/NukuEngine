#pragma once
#include"NkCommon.hpp"
#include"NkWindow.hpp"
#include"NkVulkan.hpp"
#include"NkRenderer.hpp"
#include"NkDescriptors.hpp"
#include"NkGameobject.hpp"

namespace nk{


static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}
static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

class Engine{
public:
    Engine();
    //~Engine();
    //static Engine &Instance();
    void ClearUp();
    void createMaterial();
    void deleteMaterial();
    void materialBindShader();
    void run();
    void loadModel(glm::vec3 translation, glm::vec3 scale, const std::string& filepath);
    void creatPointLight(float intensity, float radius, glm::vec3 color, glm::vec3 translation);
    void loadGameObjects();
    void setupImgui();
    static ImGui_ImplVulkanH_Window g_MainWindowData;//如果这个声明在类内，就要在类外进行初始化

private:
    Window window{ WIDTH, HEIGHT, "Vulkan Tutorial" };
    VulkanContext vulkanContext{ window };
    NkRenderer renderer{ window,vulkanContext };
    std::unique_ptr<NkDescriptorPool>globalPool{};
    NkGameObject::Map gameObjects;
};
}