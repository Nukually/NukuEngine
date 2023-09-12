//在这里写引擎接口，main.cpp只能调用这里的接口
#ifndef Nuku_h
#define Nuku_h
#include <stdlib.h>
#include "NkEngine.hpp"
/**
 * @brief 初始化引擎
 * @param width 窗口宽
 * @param height 窗口高
 */
bool NkEngineInit(int width,int height)
{
    return nk::Engine::Instance().StartUp(width, height);
}
#endif