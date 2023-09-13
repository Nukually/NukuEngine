#ifndef Nuku_h
#define Nuku_h
//在这里写引擎接口，main.cpp只能调用这里的接口
#include "NkEngine.hpp"
static std::mutex g_render_mutex;
/**
 * @brief 初始化引擎
 * @param width 窗口宽
 * @param height 窗口高
 */
bool NkEngineInit(int width,int height)
{
    return nk::Engine::Instance().StartUp(width, height);
}
/**
 * @brief Render 每帧调用
 * @param handle viewer的handle
 * @param frameTime 两帧的时间差
 * @param needResetGL 是否需要还原现有OpenGL状态
 */
void NkEngineRender()
{
    std::lock_guard<std::mutex> lock(g_render_mutex);
    nk::Engine::Instance().OnRenderFrame();
}
void NkEngineClearUp()
{
    nk::Engine::Instance().ClearUp();
}
#endif