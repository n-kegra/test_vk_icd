#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

int main() {
    VkInstance instance;
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    createInfo.pApplicationInfo = &appInfo;

    VkResult result;
    result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cout << "failed to create instance" << std::endl;
        return -1;
    }

    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(count);
    vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());

    std::cout << "Physical Device Num: " << physicalDevices.size() << std::endl;
    for (const auto &physicalDevice : physicalDevices) {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(physicalDevice, &prop);
        std::cout << prop.deviceName << std::endl;
    }

    vkDestroyInstance(instance, nullptr);

    return 0;
}
