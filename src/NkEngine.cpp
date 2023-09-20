#include"NkEngine.hpp"

#include "keyboard_movement_controller.hpp"
#include "NkBuffer.hpp"
#include "NkCamera.hpp"
#include "systems/point_light_system.hpp"
#include "systems/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace nk{
//std::mutex s_EngineInstanceMutex;
//Engine &Engine::Instance()
//{
//    static Engine *s_pEngine = nullptr;
//    if (!s_pEngine) {
//        std::unique_lock<std::mutex> lock(s_EngineInstanceMutex);
//        if (!s_pEngine) {
//            s_pEngine = new Engine();
//        }
//    }
//    return *s_pEngine;
//}
Engine::Engine() {
    globalPool =
        NkDescriptorPool::Builder(vulkanContext)
        .setMaxSets(NkSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, NkSwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    loadGameObjects();
}
bool Engine::StartUp(int width,int height)
{
    std::cout<<"hello world"<<std::endl;
    //m_Window = new Window(width,height,"hello world");
    //m_Vulkan = new VulkanContext(*m_Window);
    //m_renderer = new NkRenderer(*m_Window, *m_Vulkan);
    return true;
}
void Engine::OnRenderFrame()
{
    while (!glfwWindowShouldClose(window.getGLFWwindow()))
    {
        glfwPollEvents();
        drawFrame();
    }
    vulkanContext.waitIdle();
}
void Engine::ClearUp()
{
    //todo:如何正确且自动地控制析构顺序？
    //delete m_Vulkan;
    //delete m_Window;
    return;
}
void Engine::drawFrame(){
    //m_Vulkan->drawFrame();
}
void Engine::run() {
    std::vector<std::unique_ptr<NkBuffer>> uboBuffers(NkSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<NkBuffer>(
            vulkanContext,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout =
        NkDescriptorSetLayout::Builder(vulkanContext)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
        .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(NkSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        NkDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{
        vulkanContext,
        renderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout() };
    PointLightSystem pointLightSystem{
        vulkanContext,
        renderer.getSwapChainRenderPass(),
        globalSetLayout->getDescriptorSetLayout() };
    NkCamera camera{};

    auto viewerObject = NkGameObject::createGameObject();
    viewerObject.transform.translation.z = -2.5f;
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!window.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{
                frameIndex,
                frameTime,
                commandBuffer,
                camera,
                globalDescriptorSets[frameIndex],
                gameObjects };

            // update
            GlobalUbo ubo{};
            ubo.projection = camera.getProjection();
            ubo.view = camera.getView();
            ubo.inverseView = camera.getInverseView();
            pointLightSystem.update(frameInfo, ubo);
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            renderer.beginSwapChainRenderPass(commandBuffer);

            // order here matters
            simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);

            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }

    vkDeviceWaitIdle(vulkanContext.device());
}
void Engine::loadGameObjects() {
    std::shared_ptr<NkModel> model =
        NkModel::createModelFromFile(vulkanContext, "models/flat_vase.obj");
    auto flatVase = NkGameObject::createGameObject();
    flatVase.model = model;
    flatVase.transform.translation = { -.5f, .5f, 0.f };
    flatVase.transform.scale = { 3.f, 1.5f, 3.f };
    gameObjects.emplace(flatVase.getId(), std::move(flatVase));

    model = NkModel::createModelFromFile(vulkanContext, "models/smooth_vase.obj");
    auto smoothVase = NkGameObject::createGameObject();
    smoothVase.model = model;
    smoothVase.transform.translation = { .5f, .5f, 0.f };
    smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
    gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

    model = NkModel::createModelFromFile(vulkanContext, "models/quad.obj");
    auto floor = NkGameObject::createGameObject();
    floor.model = model;
    floor.transform.translation = { 0.f, .5f, 0.f };
    floor.transform.scale = { 3.f, 1.f, 3.f };
    gameObjects.emplace(floor.getId(), std::move(floor));

    std::vector<glm::vec3> lightColors{
        {1.f, .1f, .1f},
        { .1f, .1f, 1.f },
        { .1f, 1.f, .1f },
        { 1.f, 1.f, .1f },
        { .1f, 1.f, 1.f },
        { 1.f, 1.f, 1.f }  //
    };

    for (int i = 0; i < lightColors.size(); i++) {
        auto pointLight = NkGameObject::makePointLight(0.2f);
        pointLight.color = lightColors[i];
        auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, -1.f, 0.f });
        pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        gameObjects.emplace(pointLight.getId(), std::move(pointLight));
    }
}
}