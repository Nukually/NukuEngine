#include"NkEngine.hpp"
#include<mutex>

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
    return true;
}
}