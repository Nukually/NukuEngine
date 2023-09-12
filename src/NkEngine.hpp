#pragma once
//�������Engine��
#include"NkCommon.hpp"
#include"NkWindow.hpp"
namespace nk{
class Engine{
public:
    ~Engine();

    static Engine &Instance();

    bool StartUp(int width, int height);
    void OnRenderFrame();
private:
    int m_width;
    int m_height;
    Window* m_Window;
    //NkDevice nkDevice{ nkWindow };
    //NkRenderer nkRenderer{ nkWindow, nkDevice };
};
}