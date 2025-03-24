#ifndef LOGICAL_SWAPCHAIN_HPP_INCLUDED
#define LOGICAL_SWAPCHAIN_HPP_INCLUDED
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "effect.hpp"

#include "vulkan_include.hpp"

#include "logical_device.hpp"

#include "display_server.hpp" 

namespace vkPost
{
    // for each swapchain, we have the Images and the other stuff we need to execute the compute shader
class LogicalSwapchain {
public:
    LogicalDevice* pLogicalDevice;
    VkSwapchainCreateInfoKHR swapchainCreateInfo;
    VkExtent2D imageExtent;
    VkFormat format;
    uint32_t imageCount;
    std::vector<VkImage> images;
    std::vector<VkImage> fakeImages;
    VkDeviceMemory fakeImageMemory;
    std::vector<std::shared_ptr<Effect>> effects;
    std::vector<VkCommandBuffer> commandBuffersEffect;
    std::vector<VkCommandBuffer> commandBuffersNoEffect;
    std::vector<VkSemaphore> semaphores;
    std::shared_ptr<Effect> defaultTransfer;
    DisplayServer display_server = DisplayServer::UNKNOWN;

    void destroy();
};
} // namespace vkPost

#endif // LOGICAL_SWAPCHAIN_HPP_INCLUDED
