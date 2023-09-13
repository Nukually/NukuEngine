#pragma once
//�������Engine��
#include"NkCommon.hpp"
#include"NkWindow.hpp"
#include"NkVulkan.hpp"
namespace nk{
class Engine{
public:
    ~Engine();
    static Engine &Instance();
    bool StartUp(int width, int height);
    void OnRenderFrame();
    void ClearUp();
    void SetWindowForm(int width,int height);
    
private:
    int m_width;
    int m_height;
    Window* m_Window;
    VulkanContext* m_Vulkan;
    //NkDevice nkDevice{ nkWindow };
    //NkRenderer nkRenderer{ nkWindow, nkDevice };
};
}