// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#if !defined(TUTORIAL_07_HEADER)
#define TUTORIAL_07_HEADER

#include "../Common/VulkanCommon.h"
#include "../Common/Tools.h"

namespace ApiWithoutSecrets {

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
  // DescriptorParameters                                         //
  //                                                              //
  // Container class for descriptor related resources             //
  // ************************************************************ //
  struct DescriptorSetParameters {
    VkDescriptorPool                Pool;
    VkDescriptorSetLayout           Layout;
    VkDescriptorSet                 Handle;

    DescriptorSetParameters() :
      Pool( VK_NULL_HANDLE ),
      Layout( VK_NULL_HANDLE ),
      Handle( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // VertexData                                                   //
  //                                                              //
  // Struct describing data type and format of vertex attributes  //
  // ************************************************************ //
  struct VertexData {
    float   x, y, z, w;
    float   u, v;
  };

  // ************************************************************ //
  // RenderingResourcesData                                       //
  //                                                              //
  // Struct containing data used during rendering process         //
  // ************************************************************ //
  struct RenderingResourcesData {
    VkFramebuffer                         Framebuffer;
    VkCommandBuffer                       CommandBuffer;
    VkSemaphore                           ImageAvailableSemaphore;
    VkSemaphore                           FinishedRenderingSemaphore;
    VkFence                               Fence;

    RenderingResourcesData() :
      Framebuffer( VK_NULL_HANDLE ),
      CommandBuffer( VK_NULL_HANDLE ),
      ImageAvailableSemaphore( VK_NULL_HANDLE ),
      FinishedRenderingSemaphore( VK_NULL_HANDLE ),
      Fence( VK_NULL_HANDLE ) {
    }
  };

  // ************************************************************ //
  // VulkanTutorial04Parameters                                   //
  //                                                              //
  // Vulkan specific parameters                                   //
  // ************************************************************ //
  struct VulkanTutorial07Parameters {
    VkRenderPass                          RenderPass;
    ImageParameters                       Image;
    BufferParameters                      UniformBuffer;
    DescriptorSetParameters               DescriptorSet;
    VkPipelineLayout                      PipelineLayout;
    VkPipeline                            GraphicsPipeline;
    BufferParameters                      VertexBuffer;
    BufferParameters                      StagingBuffer;
    VkCommandPool                         CommandPool;
    std::vector<RenderingResourcesData>   RenderingResources;

    static const size_t                   ResourcesCount = 3;

    VulkanTutorial07Parameters();
    ~VulkanTutorial07Parameters();
  };

  // ************************************************************ //
  // Tutorial04                                                   //
  //                                                              //
  // Class for presenting Vulkan usage topics                     //
  // ************************************************************ //
  class Tutorial07 : public VulkanCommon {
  public:
    Tutorial07();
    ~Tutorial07() override;

    bool    CreateRenderingResources();
    bool    CreateStagingBuffer();
    bool    CreateTexture();
    bool    CreateUniformBuffer();
    bool    CreateDescriptorSetLayout();
    bool    CreateDescriptorPool();
    bool    AllocateDescriptorSet();
    bool    UpdateDescriptorSet();
    bool    CreateRenderPass();
    bool    CreatePipelineLayout();
    bool    CreatePipeline();
    bool    CreateVertexBuffer();

    bool    Draw() override;

  private:
    VulkanTutorial07Parameters  Vulkan;

    bool                                                              CreateCommandBuffers();
    bool                                                              CreateCommandPool(uint32_t queue_family_index, VkCommandPool *pool);
    bool                                                              AllocateCommandBuffers(VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers);
    bool                                                              CreateSemaphores();
    bool                                                              CreateFences();
    bool                                                              CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlagBits memoryProperty, BufferParameters &buffer);
    bool                                                              AllocateBufferMemory(VkBuffer buffer, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory);
    bool                                                              CreateImage( uint32_t width, uint32_t height, VkImage *image );
    bool                                                              AllocateImageMemory( VkImage image, VkMemoryPropertyFlagBits property, VkDeviceMemory *memory );
    bool                                                              CreateImageView( ImageParameters &image_parameters );
    bool                                                              CreateSampler( VkSampler *sampler );
    bool                                                              CopyTextureData( char *texture_data, uint32_t data_size, uint32_t width, uint32_t height );
    const std::array<float, 16>                                       GetUniformBufferData() const;
    bool                                                              CopyUniformBufferData();
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>     CreateShaderModule( const char* filename );
    const std::vector<float>&                                         GetVertexData() const;
    bool                                                              CopyVertexData();
    bool                                                              PrepareFrame( VkCommandBuffer command_buffer, const ImageParameters &image_parameters, VkFramebuffer &framebuffer );
    bool                                                              CreateFramebuffer( VkFramebuffer &framebuffer, VkImageView image_view );
    void                                                              DestroyBuffer( BufferParameters& buffer );

    bool                                                              ChildOnWindowSizeChanged() override;
    void                                                              ChildClear() override;
  };

} // namespace ApiWithoutSecrets

#endif // TUTORIAL_07_HEADER
