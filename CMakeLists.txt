cmake_minimum_required(VERSION 3.26)

project(test_vk_icd)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE cppfiles RELATIVE ${PROJECT_SOURCE_DIR} ./src/*.cpp)
add_library(test_vk_icd SHARED ${cppfiles} src/exports.def)
target_include_directories(test_vk_icd PRIVATE ${Vulkan_INCLUDE_DIRS})

add_executable(test_vk_icd_debug test/main.cpp)
target_include_directories(test_vk_icd_debug PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(test_vk_icd_debug PUBLIC ${Vulkan_LIBRARIES})
