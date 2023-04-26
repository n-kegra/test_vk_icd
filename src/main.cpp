#define VK_USE_PLATFORM_WIN32_KHR
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <vulkan/vk_icd.h>

struct VkInstance_T {};

struct VkPhysicalDevice_T {
    int id;
    char devicename[32];
};

std::vector<const char *> instanceExtList = {};
std::vector<VkPhysicalDevice_T> physicalDeviceList = {VkPhysicalDevice_T{1, "My Vulkan Device"}};

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(const VkInstanceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkInstance *pInstance) {
    auto p = new (std::nothrow) VkInstance_T;
    if (p == NULL)
        return VK_ERROR_INITIALIZATION_FAILED;
    *pInstance = static_cast<VkInstance>(p);
    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroyInstance(VkInstance instance, const VkAllocationCallbacks *pAllocator) {
    delete instance;
    return;
}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount, VkExtensionProperties *pProperties) {
    if (pProperties == nullptr) {
        *pPropertyCount = instanceExtList.size();
        return VK_SUCCESS;
    }
    for (size_t i = 0; i < *pPropertyCount && i < instanceExtList.size(); i++) {
        std::strcpy(pProperties[i].extensionName, instanceExtList[i]);
        pProperties[i].specVersion = 1;
    }
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(VkInstance instance, uint32_t *pPhysicalDeviceCount, VkPhysicalDevice *pPhysicalDevices) {
    if (pPhysicalDevices == nullptr) {
        *pPhysicalDeviceCount = physicalDeviceList.size();
        return VK_SUCCESS;
    }
    for (size_t i = 0; i < *pPhysicalDeviceCount && i < physicalDeviceList.size(); i++)
        pPhysicalDevices[i] = &physicalDeviceList[i];

    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties *pProperties) {
    pProperties->apiVersion = VK_API_VERSION_1_2;
    pProperties->driverVersion = VK_MAKE_VERSION(0, 1, 0);
    pProperties->deviceID = physicalDevice->id;
    pProperties->deviceType = VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
    strcpy(pProperties->deviceName, physicalDevice->devicename);
}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceVersion(uint32_t *pApiVersion) {
    *pApiVersion = VK_API_VERSION_1_2;
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vk_icdNegotiateLoaderICDInterfaceVersion(uint32_t *pVersion) { return VK_SUCCESS; }

VKAPI_ATTR VkResult VKAPI_CALL vk_dummy() {
    std::ofstream ofs("C:\\test_vk_icd\\log.txt", std::ios_base::app);
    ofs << "dummy function called" << std::endl;
    return VK_ERROR_UNKNOWN;
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_icdGetInstanceProcAddr(VkInstance instance, const char *pName) {
    if (strcmp(pName, "vkCreateInstance") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkCreateInstance);
    if (strcmp(pName, "vkDestroyInstance") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkDestroyInstance);
    if (strcmp(pName, "vkEnumerateInstanceExtensionProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceExtensionProperties);
    if (strcmp(pName, "vkEnumeratePhysicalDevices") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumeratePhysicalDevices);
    if (strcmp(pName, "vkGetPhysicalDeviceProperties") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkGetPhysicalDeviceProperties);
    if (strcmp(pName, "vkEnumerateInstanceVersion") == 0)
        return reinterpret_cast<PFN_vkVoidFunction>(vkEnumerateInstanceVersion);

    std::ofstream ofs("C:\\test_vk_icd\\log.txt", std::ios_base::app);
    ofs << "unimplemented function: " << pName << std::endl;

    return reinterpret_cast<PFN_vkVoidFunction>(vk_dummy);
}