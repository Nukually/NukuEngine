#pragma once

#include "NkBuffer.hpp"
#include "NkVulkan.hpp"

namespace nk {
class NkModel {
public:
    struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    glm::vec3 normal{};
    glm::vec2 uv{};

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

    bool operator==(const Vertex &other) const {
        return position == other.position && color == other.color && normal == other.normal &&
                uv == other.uv;
    }
    };

    struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};

    void loadModel(const std::string &filepath);
    };

    NkModel(VulkanContext &device, const NkModel::Builder &builder);
    ~NkModel();

    NkModel(const NkModel &) = delete;
    NkModel &operator=(const NkModel &) = delete;

    static std::unique_ptr<NkModel> createModelFromFile(
        VulkanContext &device, const std::string &filepath);

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);

    VulkanContext &NkDevice;

    std::unique_ptr<NkBuffer> vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<NkBuffer> indexBuffer;
    uint32_t indexCount;
};
}  // namespace nk
