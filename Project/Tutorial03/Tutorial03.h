// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#if !defined(TUTORIAL_03_HEADER)
#define TUTORIAL_03_HEADER

#include "VulkanCommon.h"
#include "Tools.h"

namespace ApiWithoutSecrets {

  // ************************************************************ //
  // VulkanTutorial03Parameters                                   //
  //                                                              //
  // Vulkan specific parameters                                   //
  // ************************************************************ //
  struct VulkanTutorial03Parameters {
    VkRenderPass                        RenderPass;
    std::vector<VkFramebuffer>          Framebuffers;
    VkPipeline                          GraphicsPipeline;
    VkSemaphore                         ImageAvailableSemaphore;
    VkSemaphore                         RenderingFinishedSemaphore;
    VkCommandPool                       GraphicsCommandPool;
    std::vector<VkCommandBuffer>        GraphicsCommandBuffers;

    VulkanTutorial03Parameters() :
      RenderPass( VK_NULL_HANDLE ),
      Framebuffers(),
      GraphicsCommandPool( VK_NULL_HANDLE ),
      GraphicsCommandBuffers(),
      GraphicsPipeline( VK_NULL_HANDLE ),
      ImageAvailableSemaphore( VK_NULL_HANDLE ),
      RenderingFinishedSemaphore( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // Tutorial03                                                   //
  //                                                              //
  // Class for presenting Vulkan usage topics                     //
  // ************************************************************ //
  class Tutorial03 : public VulkanCommon {
  public:
    Tutorial03();
    ~Tutorial03();

    bool    CreateRenderPass();
    bool    CreateFramebuffers();
    bool    CreatePipeline();
    bool    CreateSemaphores();
    bool    CreateCommandBuffers();
    bool    RecordCommandBuffers();

    bool    Draw() override;

  private:
    VulkanTutorial03Parameters  Vulkan;

    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>     CreateShaderModule( const char* filename );
    Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> CreatePipelineLayout();
    bool                                                              CreateCommandPool( uint32_t queue_family_index, VkCommandPool *pool );
    bool                                                              AllocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers );

    void                                                              ChildClear() override;
    bool                                                              ChildOnWindowSizeChanged() override;
  };

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_03_HEADER