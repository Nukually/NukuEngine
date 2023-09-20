#pragma once
#include"NkCommon.hpp"
#include"NkWindow.hpp"
#include"NkVulkan.hpp"
#include"NkRenderer.hpp"
#include"NkDescriptors.hpp"
#include"NkGameobject.hpp"
namespace nk{
class Engine{
public:
    Engine();
    //~Engine();
    //static Engine &Instance();
    bool StartUp(int width, int height);
    void OnRenderFrame();
    void ClearUp();
    void drawFrame();
    void loadGameObject();
    void removeGameObject();
    void createMaterial();
    void deleteMaterial();
    void materialBindShader();
    void run();
    void loadGameObjects();
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    
private:
    Window window{ WIDTH, HEIGHT, "Vulkan Tutorial" };
    VulkanContext vulkanContext{ window };
    NkRenderer renderer{ window,vulkanContext };
    std::unique_ptr<NkDescriptorPool>globalPool{};
    NkGameObject::Map gameObjects;
    //NkDevice nkDevice{ nkWindow };
    //NkRenderer nkRenderer{ nkWindow, nkDevice };
};
}