#include"NkEngine.hpp"
namespace nk{
std::mutex s_EngineInstanceMutex;
Engine &Engine::Instance()
{
    static Engine *s_pEngine = nullptr;
    if (!s_pEngine) {
        std::unique_lock<std::mutex> lock(s_EngineInstanceMutex);
        if (!s_pEngine) {
            s_pEngine = new Engine();
        }
    }
    return *s_pEngine;
}
bool Engine::StartUp(int width,int height)
{
    std::cout<<"hello world"<<std::endl;
    m_Window = new Window(width,height,"hello world");
    m_Vulkan = new VulkanContext();
    return true;
}
void Engine::OnRenderFrame()
{
    while (!glfwWindowShouldClose(m_Window->getGLFWwindow()))
    {
        glfwPollEvents();
    }
}
void Engine::ClearUp()
{
    //todo:如何正确且自动地控制析构顺序？
    delete m_Vulkan;
    delete m_Window;
    return;
}
void Engine::SetWindowForm(int width,int height)
{
    m_Window->m_width = width;
    m_Window->m_height = height;
}
}