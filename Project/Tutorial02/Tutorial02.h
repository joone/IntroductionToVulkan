// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#if !defined(TUTORIAL_02_HEADER)
#define TUTORIAL_02_HEADER

#include <vector>
#include <vulkan/vulkan.h>  
#include "../Common/OperatingSystem.h"

namespace ApiWithoutSecrets {

  // ************************************************************ //
  // VulkanTutorial02Parameters                                   //
  //                                                              //
  // Vulkan specific parameters                                   //
  // ************************************************************ //
  struct VulkanTutorial02Parameters {
    VulkanTutorial02Parameters();
    ~VulkanTutorial02Parameters();
    VkInstance                    Instance;
    VkPhysicalDevice              PhysicalDevice;
    VkDevice                      Device;
    VkQueue                       GraphicsQueue;
    VkQueue                       PresentQueue;
    uint32_t                      GraphicsQueueFamilyIndex;
    uint32_t                      PresentQueueFamilyIndex;
    VkSurfaceKHR                  PresentationSurface;
    VkSwapchainKHR                SwapChain;
    std::vector<VkCommandBuffer>  PresentQueueCmdBuffers;
    VkCommandPool                 PresentQueueCmdPool;
    VkSemaphore                   ImageAvailableSemaphore;
    VkSemaphore                   RenderingFinishedSemaphore;

  };

  // ************************************************************ //
  // Tutorial02                                                   //
  //                                                              //
  // Class for presenting Vulkan usage topics                     //
  // ************************************************************ //
  class Tutorial02 : public OS::TutorialBase {
  public:
    Tutorial02();
    ~Tutorial02() override;

    bool PrepareVulkan( OS::WindowParameters parameters );
    bool CreateSwapChain();
    bool OnWindowSizeChanged() override;
    bool CreateCommandBuffers();
    bool Draw() override;

    //bool CreateRenderPass();
    //bool CreateFramebuffers(); 
  private:
    //OS::LibraryHandle           VulkanLibrary;
    OS::WindowParameters        Window;
    VulkanTutorial02Parameters  Vulkan;

    bool                          LoadVulkanLibrary();
    bool                          CreateInstance();
    bool                          LoadInstanceLevelEntryPoints();
    bool                          CreatePresentationSurface();
    bool                          CreateDevice();
    bool                          CheckPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &graphics_queue_family_index, uint32_t &present_queue_family_index );
    bool                          GetDeviceQueue();
    bool                          CreateSemaphores();
    bool                          RecordCommandBuffers();
    void                          Clear();
    
   

    bool                          CheckExtensionAvailability( const char *extension_name, const std::vector<VkExtensionProperties> &available_extensions );
    uint32_t                      GetSwapChainNumImages( VkSurfaceCapabilitiesKHR &surface_capabilities );
    VkSurfaceFormatKHR            GetSwapChainFormat( std::vector<VkSurfaceFormatKHR> &surface_formats );
    VkExtent2D                    GetSwapChainExtent( VkSurfaceCapabilitiesKHR &surface_capabilities );
    VkImageUsageFlags             GetSwapChainUsageFlags( VkSurfaceCapabilitiesKHR &surface_capabilities );
    VkSurfaceTransformFlagBitsKHR GetSwapChainTransform( VkSurfaceCapabilitiesKHR &surface_capabilities );
    VkPresentModeKHR              GetSwapChainPresentMode( std::vector<VkPresentModeKHR> &present_modes );
  };

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_02_HEADER
