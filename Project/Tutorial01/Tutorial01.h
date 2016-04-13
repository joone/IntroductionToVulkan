// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#if !defined(TUTORIAL_01_HEADER)
#define TUTORIAL_01_HEADER

#include "vulkan.h"
#include "OperatingSystem.h"

namespace ApiWithoutSecrets {

  // ************************************************************ //
  // VulkanTutorial01Parameters                                   //
  //                                                              //
  // Vulkan specific parameters                                   //
  // ************************************************************ //
  struct VulkanTutorial01Parameters {
    VkInstance  Instance;
    VkDevice    Device;
    uint32_t    QueueFamilyIndex;
    VkQueue     Queue;

    VulkanTutorial01Parameters() :
      Instance( VK_NULL_HANDLE ),
      Device( VK_NULL_HANDLE ),
      QueueFamilyIndex( 0 ),
      Queue( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // Tutorial01                                                   //
  //                                                              //
  // Class for presenting Vulkan usage topics                     //
  // ************************************************************ //
  class Tutorial01 : public OS::TutorialBase {
  public:
    Tutorial01();
    ~Tutorial01();

    bool OnWindowSizeChanged() override;
    bool Draw() override;

    bool PrepareVulkan();

  private:
    OS::LibraryHandle           VulkanLibrary;
    VulkanTutorial01Parameters  Vulkan;

    bool LoadVulkanLibrary();
    bool LoadExportedEntryPoints();
    bool LoadGlobalLevelEntryPoints();
    bool CreateInstance();
    bool LoadInstanceLevelEntryPoints();
    bool CreateDevice();
    bool CheckPhysicalDeviceProperties( VkPhysicalDevice physical_device, uint32_t &queue_family_index );
    bool LoadDeviceLevelEntryPoints();
    bool GetDeviceQueue();
  };

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_01_HEADER