#ifndef EFFECT_DEBAND_HPP_INCLUDED
#define EFFECT_DEBAND_HPP_INCLUDED
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "vulkan_include.hpp"

#include "effect_simple.hpp"
#include "config.hpp"

namespace vkPost
{
    class DebandEffect : public SimpleEffect
    {
    public:
        DebandEffect(LogicalDevice*       pLogicalDevice,
                     VkFormat             format,
                     VkExtent2D           imageExtent,
                     std::vector<VkImage> inputImages,
                     std::vector<VkImage> outputImages,
                     Config*              pConfig);
        ~DebandEffect();
    };
} // namespace vkPost

#endif // EFFECT_DEBAND_HPP_INCLUDED
