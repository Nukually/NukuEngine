#pragma once

#include "NkVulkan.hpp"


namespace nk {

class NkDescriptorSetLayout {
public:
    class Builder {
    public:
        Builder(VulkanContext &device) : device{device} {}

        Builder &addBinding(
            uint32_t binding,
            VkDescriptorType descriptorType,
            VkShaderStageFlags stageFlags,
            uint32_t count = 1);
        std::unique_ptr<NkDescriptorSetLayout> build() const;

    private:
        VulkanContext &device;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
    };

    NkDescriptorSetLayout(
        VulkanContext &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~NkDescriptorSetLayout();
    NkDescriptorSetLayout(const NkDescriptorSetLayout &) = delete;
    NkDescriptorSetLayout &operator=(const NkDescriptorSetLayout &) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VulkanContext &device;
    VkDescriptorSetLayout descriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

    friend class NkDescriptorWriter;
};

class NkDescriptorPool {
public:
    class Builder {
    public:
        Builder(VulkanContext &device) : device{device} {}

        Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
        Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
        Builder &setMaxSets(uint32_t count);
        std::unique_ptr<NkDescriptorPool> build() const;

    private:
        VulkanContext &device;
        std::vector<VkDescriptorPoolSize> poolSizes{};
        uint32_t maxSets = 1000;
        VkDescriptorPoolCreateFlags poolFlags = 0;
    };

    NkDescriptorPool(
        VulkanContext &device,
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~NkDescriptorPool();
    NkDescriptorPool(const NkDescriptorPool &) = delete;
    NkDescriptorPool &operator=(const NkDescriptorPool &) = delete;

    bool allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void resetPool();

private:
    VulkanContext &device;
    VkDescriptorPool descriptorPool;

    friend class NkDescriptorWriter;
};

class NkDescriptorWriter {
public:
    NkDescriptorWriter(NkDescriptorSetLayout &setLayout, NkDescriptorPool &pool);

    NkDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    NkDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);

private:
    NkDescriptorSetLayout &setLayout;
    NkDescriptorPool &pool;
    std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace Nk
