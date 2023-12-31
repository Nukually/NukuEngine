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
//先构造这个类里的成员再调用构造函数！！！
    globalPool =
        NkDescriptorPool::Builder(vulkanContext)
        .setMaxSets(NkSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, NkSwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    loadGameObjects();
    //setupImgui();
}

void Engine::ClearUp()
{
    vulkanContext.cleanup();
    //TODO:如何正确且自动地控制析构顺序？
    //delete m_Vulkan;
    //delete m_Window;
    return;
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

        /*ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow((bool*)true);
        ImGui::Render();*/

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
void Engine::loadModel(glm::vec3 translation, glm::vec3 scale, const std::string& filepath) {
    std::shared_ptr<NkModel> model =
        NkModel::createModelFromFile(vulkanContext, filepath);
    auto go = NkGameObject::createGameObject();
    go.model = model;
    go.transform.translation = translation;
    go.transform.scale = scale;
    gameObjects.emplace(go.getId(), std::move(go));
}
void Engine::creatPointLight(float intensity, float radius, glm::vec3 color, glm::vec3 translation) {
    auto pointLight = NkGameObject::makePointLight(intensity, radius, color);
    pointLight.transform.translation = translation;
    gameObjects.emplace(pointLight.getId(), std::move(pointLight));
}
void Engine::loadGameObjects() {

    loadModel({ -.5f, .5f, 0.f }, { 3.f, 1.5f, 3.f }, "models/flat_vase.obj");
    loadModel({ .5f, .5f, 0.f }, { 3.f, 1.5f, 5.f }, "models/smooth_vase.obj");
    loadModel({ 0.f, .5f, 0.f }, { 3.f, 1.f, 3.f }, "models/quad.obj");

    std::vector<glm::vec3> lightColors{
        {1.f, .1f, .1f},
        { .1f, .1f, 1.f },
        { .1f, 1.f, .1f },
        { 1.f, 1.f, .1f },
        { .1f, 1.f, 1.f },
        { 1.f, 1.f, 1.f }  //
    };
    for (int i = 0; i < lightColors.size(); i++) {
        auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, -1.f, 0.f });
        creatPointLight(0.2f, 0.1f, lightColors[i], glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f)));
    }

}
ImGui_ImplVulkanH_Window Engine::g_MainWindowData;
//void Engine::setupImgui()
//{
//    ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
//    
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls        // IF using Docking Branch
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//
//    ImGui_ImplGlfw_InitForVulkan(window.getGLFWwindow(), true);
//    ImGui_ImplVulkan_InitInfo init_info = {};
//    init_info.Instance = vulkanContext.getInstance();
//    init_info.PhysicalDevice = vulkanContext.getPhysicalDevice();
//    init_info.Device = vulkanContext.getDevice();
//    init_info.QueueFamily = vulkanContext.findQueueFamilies(vulkanContext.getPhysicalDevice()).presentFamily.value();
//    init_info.Queue = vulkanContext.getVkQueue();
//    init_info.PipelineCache = VK_NULL_HANDLE;
//    init_info.DescriptorPool = globalPool->getVkDescriptorPool();
//    init_info.Subpass = 0;
//    init_info.MinImageCount = 2;
//    init_info.ImageCount = 20;//wd->ImageCount
//    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
//    init_info.Allocator = nullptr;
//    init_info.CheckVkResultFn = check_vk_result;
//    //auto _renderpass = renderer.
//    ImGui_ImplVulkan_Init(&init_info, renderer.getSwapChainRenderPass());
//
//    // Upload Fonts
//    {
//        // Use any command queue
//        VkCommandPool command_pool = vulkanContext.getCommandPool();
//        VkCommandBuffer command_buffer = renderer.beginFrame();
//
//        auto err = vkResetCommandPool(vulkanContext.getDevice(), command_pool, 0);
//        check_vk_result(err);
//        VkCommandBufferBeginInfo begin_info = {};
//        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//        err = vkBeginCommandBuffer(command_buffer, &begin_info);
//        check_vk_result(err);
//
//        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
//
//        VkSubmitInfo end_info = {};
//        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//        end_info.commandBufferCount = 1;
//        end_info.pCommandBuffers = &command_buffer;
//        err = vkEndCommandBuffer(command_buffer);
//        check_vk_result(err);
//        err = vkQueueSubmit(vulkanContext.getVkQueue(), 1, &end_info, VK_NULL_HANDLE);
//        check_vk_result(err);
//
//        err = vkDeviceWaitIdle(vulkanContext.getDevice());
//        check_vk_result(err);
//        ImGui_ImplVulkan_DestroyFontUploadObjects();
//    }
//    
//}
}