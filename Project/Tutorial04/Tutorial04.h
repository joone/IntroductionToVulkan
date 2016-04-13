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
  // ImageParameters                                              //
  //                                                              //
  // Vulkan Image's parameters container class                    //
  // ************************************************************ //
  struct ImageParameters {
    VkFormat                        Format;
    VkImage                         Handle;
    VkDeviceMemory                  Memory;
    VkImageView                     View;

    ImageParameters() :
      Format( VK_FORMAT_R8G8B8A8_UNORM ),
      Handle( VK_NULL_HANDLE ),
      Memory( VK_NULL_HANDLE ),
      View( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // BufferParameters                                             //
  //                                                              //
  // Vulkan Buffer's parameters container class                   //
  // ************************************************************ //
  struct BufferParameters {
    VkBuffer                        Handle;
    VkDeviceMemory                  Memory;
    uint32_t                        Size;

    BufferParameters() :
      Handle( VK_NULL_HANDLE ),
      Memory( VK_NULL_HANDLE ),
      Size( 0 ) {
    }
  };

  // ************************************************************ //
  // VertexData                                                   //
  //                                                              //
  // Struct describing data type and format of vertex attributes  //
  // ************************************************************ //
  struct VertexData {
    float   x, y, z, w;
    float   r, g, b, a;
  };

  // ************************************************************ //
  // VulkanTutorial04Parameters                                   //
  //                                                              //
  // Vulkan specific parameters                                   //
  // ************************************************************ //
  struct VulkanTutorial04Parameters {
    VkRenderPass                    RenderPass;
    ImageParameters                 Image;
    VkFramebuffer                   Framebuffer;
    VkPipeline                      GraphicsPipeline;
    BufferParameters                VertexBuffer;
    VkFence                         Fence;
    VkCommandPool                   GraphicsCommandPool;
    VkCommandBuffer                 RenderingCommandBuffer;
    VkCommandBuffer                 CopyingCommandBuffer;

    VulkanTutorial04Parameters() :
      RenderPass( VK_NULL_HANDLE ),
      Image(),
      GraphicsPipeline( VK_NULL_HANDLE ),
      VertexBuffer(),
      Fence( VK_NULL_HANDLE ),
      GraphicsCommandPool( VK_NULL_HANDLE ),
      RenderingCommandBuffer( VK_NULL_HANDLE ),
      CopyingCommandBuffer( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // Tutorial04                                                   //
  //                                                              //
  // Class for presenting Vulkan usage topics                     //
  // ************************************************************ //
  class Tutorial04 : public VulkanCommon {
  public:
    Tutorial04();
    ~Tutorial04();

    bool    CreateRenderPass();
    bool    CreateImage();
    bool    CreateFramebuffer();
    bool    CreatePipeline();
    bool    CreateVertexBuffer();
    bool    CreateFence();
    bool    CreateCommandBuffers();
    bool    RecordRenderingCommandBuffer();

    bool    Draw() override;

  private:
    VulkanTutorial04Parameters  Vulkan;

    bool                                                              AllocateImageMemory( VkImage image, VkDeviceMemory *memory );
    bool                                                              AllocateBufferMemory( VkBuffer buffer, VkDeviceMemory *memory );
    bool                                                              CommitMemoryChanges( VkBuffer buffer, VkDeviceSize size );
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>     CreateShaderModule( const char* filename );
    Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> CreatePipelineLayout();
    bool                                                              CreateCommandPool( uint32_t queue_family_index, VkCommandPool *pool );
    bool                                                              AllocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers );
    bool                                                              RecordCopyingCommandBuffer( VkImage swap_chain_image );

    void                                                              ChildClear() override;
    bool                                                              ChildOnWindowSizeChanged() override;
  };

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_03_HEADER