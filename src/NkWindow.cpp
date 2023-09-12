#include"NkWindow.hpp"

namespace nk{
Window::Window(int width,int height,std::string name):m_width{width},m_height{height},windowName{name}{
    initWindow();
}
Window::~Window(){
    glfwDestroyWindow(window);
    glfwTerminate();  
}
void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window = glfwCreateWindow(m_width, m_height, windowName.c_str(), nullptr, nullptr);
  //glfwSetWindowUserPointer(window, this);
  //glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}
void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to craete window surface");
  }
}
void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  auto nkWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  nkWindow->framebufferResized = true;
  nkWindow->m_width = width;
  nkWindow->m_height = height;
}
}