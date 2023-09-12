#pragma once
//这里设计Engine类
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include<glm/vec4.hpp>
#include<glm/mat4x4.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
namespace nk{
class Engine{
public:
    ~Engine();

    static Engine &Instance();

    bool StartUp(int width, int height);
private:
    //NkWindow nkWindow{ WIDTH,HEIGHT,"hello world" };
    //NkDevice nkDevice{ nkWindow };
    //NkRenderer nkRenderer{ nkWindow, nkDevice };
};
}