#ifndef NkWindow_h
#define NkWindow_h
#include"NkCommon.hpp"
namespace nk{
class Window{
public:
    Window(int width,int height,std::string name);
    Window(const Window&)=delete;
    Window &operator=(const Window&)=delete;
    ~Window();
    bool wasWindowResized() { return framebufferResized; }
    GLFWwindow *getGLFWwindow() const { return window; }
    bool shouldClose() { return glfwWindowShouldClose(window); }
    VkExtent2D getExtent() { return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
private:
    void initWindow();
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
private:
  int m_width;
  int m_height;
  bool framebufferResized = false;

  std::string windowName;
  GLFWwindow *window;
};
}
#endif