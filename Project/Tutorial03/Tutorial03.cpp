// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#include "Tutorial03.h"
#include "../Common/vulkan_shader_module.h"

namespace ApiWithoutSecrets {
  VulkanTutorial03Parameters::~VulkanTutorial03Parameters() { }
  VulkanTutorial03Parameters::VulkanTutorial03Parameters() :
      RenderPass( VK_NULL_HANDLE ),
      Framebuffers(),
      GraphicsPipeline( VK_NULL_HANDLE ),
      ImageAvailableSemaphore( VK_NULL_HANDLE ),
      RenderingFinishedSemaphore( VK_NULL_HANDLE ),
      GraphicsCommandPool( VK_NULL_HANDLE ),
      GraphicsCommandBuffers() {
   }

  Tutorial03::Tutorial03() :
    Vulkan() {
  }

  bool Tutorial03::CreateRenderPass() {
    VkAttachmentDescription attachment_descriptions[] = {
      {
        0,                                          // VkAttachmentDescriptionFlags   flags
        GetSwapChain().Format,                      // VkFormat                       format
        VK_SAMPLE_COUNT_1_BIT,                      // VkSampleCountFlagBits          samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                // VkAttachmentLoadOp             loadOp
        VK_ATTACHMENT_STORE_OP_STORE,               // VkAttachmentStoreOp            storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,            // VkAttachmentLoadOp             stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,           // VkAttachmentStoreOp            stencilStoreOp
        VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                  initialLayout;
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR             // VkImageLayout                  finalLayout
      }
    };

    VkAttachmentReference color_attachment_references[] = {
      {
        0,                                          // uint32_t                       attachment
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL    // VkImageLayout                  layout
      }
    };

    VkSubpassDescription subpass_descriptions[] = {
      {
        0,                                          // VkSubpassDescriptionFlags      flags
        VK_PIPELINE_BIND_POINT_GRAPHICS,            // VkPipelineBindPoint            pipelineBindPoint
        0,                                          // uint32_t                       inputAttachmentCount
        nullptr,                                    // const VkAttachmentReference   *pInputAttachments
        1,                                          // uint32_t                       colorAttachmentCount
        color_attachment_references,                // const VkAttachmentReference   *pColorAttachments
        nullptr,                                    // const VkAttachmentReference   *pResolveAttachments
        nullptr,                                    // const VkAttachmentReference   *pDepthStencilAttachment
        0,                                          // uint32_t                       preserveAttachmentCount
        nullptr                                     // const uint32_t*                pPreserveAttachments
      }
    };

    VkRenderPassCreateInfo render_pass_create_info = {
      VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,    // VkStructureType                sType
      nullptr,                                      // const void                    *pNext
      0,                                            // VkRenderPassCreateFlags        flags
      1,                                            // uint32_t                       attachmentCount
      attachment_descriptions,                      // const VkAttachmentDescription *pAttachments
      1,                                            // uint32_t                       subpassCount
      subpass_descriptions,                         // const VkSubpassDescription    *pSubpasses
      0,                                            // uint32_t                       dependencyCount
      nullptr                                       // const VkSubpassDependency     *pDependencies
    };

    if( vkCreateRenderPass( GetDevice(), &render_pass_create_info, nullptr, &Vulkan.RenderPass ) != VK_SUCCESS ) {
      std::cout << "Could not create render pass!" << std::endl;
      return false;
    }

    return true;
  }

  bool Tutorial03::CreateFramebuffers() {
    const std::vector<ImageParameters> &swap_chain_images = GetSwapChain().Images;
    Vulkan.Framebuffers.resize( swap_chain_images.size() );

    for( size_t i = 0; i < swap_chain_images.size(); ++i ) {
      VkFramebufferCreateInfo framebuffer_create_info = {
        VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // VkStructureType                sType
        nullptr,                                    // const void                    *pNext
        0,                                          // VkFramebufferCreateFlags       flags
        Vulkan.RenderPass,                          // VkRenderPass                   renderPass
        1,                                          // uint32_t                       attachmentCount
        &swap_chain_images[i].View,                 // const VkImageView             *pAttachments
        300,                                        // uint32_t                       width
        300,                                        // uint32_t                       height
        1                                           // uint32_t                       layers
      };

      if( vkCreateFramebuffer( GetDevice(), &framebuffer_create_info, nullptr, &Vulkan.Framebuffers[i] ) != VK_SUCCESS ) {
        std::cout << "Could not create a framebuffer!" << std::endl;
        return false;
      }
    }
    return true;
  }

  Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> Tutorial03::CreatePipelineLayout() {
    VkPipelineLayoutCreateInfo layout_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,  // VkStructureType                sType
      nullptr,                                        // const void                    *pNext
      0,                                              // VkPipelineLayoutCreateFlags    flags
      0,                                              // uint32_t                       setLayoutCount
      nullptr,                                        // const VkDescriptorSetLayout   *pSetLayouts
      0,                                              // uint32_t                       pushConstantRangeCount
      nullptr                                         // const VkPushConstantRange     *pPushConstantRanges
    };

    VkPipelineLayout pipeline_layout;
    if( vkCreatePipelineLayout( GetDevice(), &layout_create_info, nullptr, &pipeline_layout ) != VK_SUCCESS ) {
      std::cout << "Could not create pipeline layout!" << std::endl;
      return Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout>();
    }

    return Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout>( pipeline_layout, vkDestroyPipelineLayout, GetDevice() );
  }

  bool Tutorial03::CreatePipeline() {
   const std::string kVertexShaderSource = 
       "#version 450\n"
       "out gl_PerVertex\n"
       "{"
       "  vec4 gl_Position;"
       "};\n"
       "void main() {"
       "    vec2 pos[3] = vec2[3]( vec2(-0.7, 0.7), vec2(0.7, 0.7), vec2(0.0, -0.7) );"
       "    gl_Position = vec4( pos[gl_VertexIndex], 0.0, 1.0 );"
       "}";
    VulkanShaderModule vertex_shader_module(GetDevice());
    vertex_shader_module.InitializeGLSL(
      VulkanShaderModule::ShaderType::VERTEX,"vetext" , "main", kVertexShaderSource);

    const std::string kFragShaderSource = 
      "#version 450\n"
      "layout(location = 0) out vec4 out_Color;\n"
      "void main() {"
      "  out_Color = vec4( 0.0, 0.4, 1.0, 1.0 );\n"
      "}";
    VulkanShaderModule fragment_shader_module(GetDevice());
    fragment_shader_module.InitializeGLSL(
      VulkanShaderModule::ShaderType::FRAGMENT,"fragment" , "main", kFragShaderSource);

    if( !vertex_shader_module.IsValid()) {
      std::cout << "vertext shader error = " << vertex_shader_module.GetErrorMessages();
      return false;
    }

    if( !fragment_shader_module.IsValid() ) {
      std::cout << "fragment shader error = " << fragment_shader_module.GetErrorMessages();
      return false;
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos = {
      // Vertex shader
      {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
        nullptr,                                                    // const void                                    *pNext
        0,                                                          // VkPipelineShaderStageCreateFlags               flags
        VK_SHADER_STAGE_VERTEX_BIT,                                 // VkShaderStageFlagBits                          stage
        vertex_shader_module.handle(),                                 // VkShaderModule                                 module
        "main",                                                     // const char                                    *pName
        nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
      },
      // Fragment shader
      {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
        nullptr,                                                    // const void                                    *pNext
        0,                                                          // VkPipelineShaderStageCreateFlags               flags
        VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlagBits                          stage
        fragment_shader_module.handle(),                               // VkShaderModule                                 module
        "main",                                                     // const char                                    *pName
        nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,    // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineVertexInputStateCreateFlags          flags;
      0,                                                            // uint32_t                                       vertexBindingDescriptionCount
      nullptr,                                                      // const VkVertexInputBindingDescription         *pVertexBindingDescriptions
      0,                                                            // uint32_t                                       vertexAttributeDescriptionCount
      nullptr                                                       // const VkVertexInputAttributeDescription       *pVertexAttributeDescriptions
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,  // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineInputAssemblyStateCreateFlags        flags
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,                          // VkPrimitiveTopology                            topology
      VK_FALSE                                                      // VkBool32                                       primitiveRestartEnable
    };

    VkViewport viewport = {
      0.0f,                                                         // float                                          x
      0.0f,                                                         // float                                          y
      300.0f,                                                       // float                                          width
      300.0f,                                                       // float                                          height
      0.0f,                                                         // float                                          minDepth
      1.0f                                                          // float                                          maxDepth
    };

    VkRect2D scissor = {
      {                                                             // VkOffset2D                                     offset
        0,                                                            // int32_t                                        x
        0                                                             // int32_t                                        y
      },
      {                                                             // VkExtent2D                                     extent
        300,                                                          // int32_t                                        width
        300                                                           // int32_t                                        height
      }
    };

    VkPipelineViewportStateCreateInfo viewport_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,        // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineViewportStateCreateFlags             flags
      1,                                                            // uint32_t                                       viewportCount
      &viewport,                                                    // const VkViewport                              *pViewports
      1,                                                            // uint32_t                                       scissorCount
      &scissor                                                      // const VkRect2D                                *pScissors
    };

    VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,   // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineRasterizationStateCreateFlags        flags
      VK_FALSE,                                                     // VkBool32                                       depthClampEnable
      VK_FALSE,                                                     // VkBool32                                       rasterizerDiscardEnable
      VK_POLYGON_MODE_FILL,                                         // VkPolygonMode                                  polygonMode
      VK_CULL_MODE_BACK_BIT,                                        // VkCullModeFlags                                cullMode
      VK_FRONT_FACE_COUNTER_CLOCKWISE,                              // VkFrontFace                                    frontFace
      VK_FALSE,                                                     // VkBool32                                       depthBiasEnable
      0.0f,                                                         // float                                          depthBiasConstantFactor
      0.0f,                                                         // float                                          depthBiasClamp
      0.0f,                                                         // float                                          depthBiasSlopeFactor
      1.0f                                                          // float                                          lineWidth
    };

    VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,     // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineMultisampleStateCreateFlags          flags
      VK_SAMPLE_COUNT_1_BIT,                                        // VkSampleCountFlagBits                          rasterizationSamples
      VK_FALSE,                                                     // VkBool32                                       sampleShadingEnable
      1.0f,                                                         // float                                          minSampleShading
      nullptr,                                                      // const VkSampleMask                            *pSampleMask
      VK_FALSE,                                                     // VkBool32                                       alphaToCoverageEnable
      VK_FALSE                                                      // VkBool32                                       alphaToOneEnable
    };

    VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
      VK_FALSE,                                                     // VkBool32                                       blendEnable
      VK_BLEND_FACTOR_ONE,                                          // VkBlendFactor                                  srcColorBlendFactor
      VK_BLEND_FACTOR_ZERO,                                         // VkBlendFactor                                  dstColorBlendFactor
      VK_BLEND_OP_ADD,                                              // VkBlendOp                                      colorBlendOp
      VK_BLEND_FACTOR_ONE,                                          // VkBlendFactor                                  srcAlphaBlendFactor
      VK_BLEND_FACTOR_ZERO,                                         // VkBlendFactor                                  dstAlphaBlendFactor
      VK_BLEND_OP_ADD,                                              // VkBlendOp                                      alphaBlendOp
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |         // VkColorComponentFlags                          colorWriteMask
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,     // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineColorBlendStateCreateFlags           flags
      VK_FALSE,                                                     // VkBool32                                       logicOpEnable
      VK_LOGIC_OP_COPY,                                             // VkLogicOp                                      logicOp
      1,                                                            // uint32_t                                       attachmentCount
      &color_blend_attachment_state,                                // const VkPipelineColorBlendAttachmentState     *pAttachments
      { 0.0f, 0.0f, 0.0f, 0.0f }                                    // float                                          blendConstants[4]
    };

    Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> pipeline_layout = CreatePipelineLayout();
    if( !pipeline_layout ) {
      return false;
    }

    VkGraphicsPipelineCreateInfo pipeline_create_info = {
      VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,              // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineCreateFlags                          flags
      static_cast<uint32_t>(shader_stage_create_infos.size()),      // uint32_t                                       stageCount
      &shader_stage_create_infos[0],                                // const VkPipelineShaderStageCreateInfo         *pStages
      &vertex_input_state_create_info,                              // const VkPipelineVertexInputStateCreateInfo    *pVertexInputState;
      &input_assembly_state_create_info,                            // const VkPipelineInputAssemblyStateCreateInfo  *pInputAssemblyState
      nullptr,                                                      // const VkPipelineTessellationStateCreateInfo   *pTessellationState
      &viewport_state_create_info,                                  // const VkPipelineViewportStateCreateInfo       *pViewportState
      &rasterization_state_create_info,                             // const VkPipelineRasterizationStateCreateInfo  *pRasterizationState
      &multisample_state_create_info,                               // const VkPipelineMultisampleStateCreateInfo    *pMultisampleState
      nullptr,                                                      // const VkPipelineDepthStencilStateCreateInfo   *pDepthStencilState
      &color_blend_state_create_info,                               // const VkPipelineColorBlendStateCreateInfo     *pColorBlendState
      nullptr,                                                      // const VkPipelineDynamicStateCreateInfo        *pDynamicState
      pipeline_layout.Get(),                                        // VkPipelineLayout                               layout
      Vulkan.RenderPass,                                            // VkRenderPass                                   renderPass
      0,                                                            // uint32_t                                       subpass
      VK_NULL_HANDLE,                                               // VkPipeline                                     basePipelineHandle
      -1                                                            // int32_t                                        basePipelineIndex
    };

    if( vkCreateGraphicsPipelines( GetDevice(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &Vulkan.GraphicsPipeline ) != VK_SUCCESS ) {
      std::cout << "Could not create graphics pipeline!" << std::endl;
      return false;
    }

    vertex_shader_module.Destroy();
    fragment_shader_module.Destroy();
    return true;
  }

  bool Tutorial03::CreateSemaphores() {
    VkSemaphoreCreateInfo semaphore_create_info = {
      VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,      // VkStructureType          sType
      nullptr,                                      // const void*              pNext
      0                                             // VkSemaphoreCreateFlags   flags
    };

    if( (vkCreateSemaphore( GetDevice(), &semaphore_create_info, nullptr, &Vulkan.ImageAvailableSemaphore ) != VK_SUCCESS) ||
        (vkCreateSemaphore( GetDevice(), &semaphore_create_info, nullptr, &Vulkan.RenderingFinishedSemaphore ) != VK_SUCCESS) ) {
      std::cout << "Could not create semaphores!" << std::endl;
      return false;
    }

    return true;
  }

  bool Tutorial03::CreateCommandBuffers() {
    if( !CreateCommandPool( GetGraphicsQueue().FamilyIndex, &Vulkan.GraphicsCommandPool ) ) {
      std::cout << "Could not create command pool!" << std::endl;
      return false;
    }

    uint32_t image_count = static_cast<uint32_t>(GetSwapChain().Images.size());
    Vulkan.GraphicsCommandBuffers.resize( image_count, VK_NULL_HANDLE );

    if( !AllocateCommandBuffers( Vulkan.GraphicsCommandPool, image_count, &Vulkan.GraphicsCommandBuffers[0] ) ) {
      std::cout << "Could not allocate command buffers!" << std::endl;
      return false;
    }
    return true;
  }

  bool Tutorial03::CreateCommandPool( uint32_t queue_family_index, VkCommandPool *pool ) {
    VkCommandPoolCreateInfo cmd_pool_create_info = {
      VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,     // VkStructureType                sType
      nullptr,                                        // const void                    *pNext
      0,                                              // VkCommandPoolCreateFlags       flags
      queue_family_index                              // uint32_t                       queueFamilyIndex
    };

    if( vkCreateCommandPool( GetDevice(), &cmd_pool_create_info, nullptr, pool ) != VK_SUCCESS ) {
      return false;
    }
    return true;
  }

  bool Tutorial03::AllocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers ) {
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, // VkStructureType                sType
      nullptr,                                        // const void                    *pNext
      pool,                                           // VkCommandPool                  commandPool
      VK_COMMAND_BUFFER_LEVEL_PRIMARY,                // VkCommandBufferLevel           level
      count                                           // uint32_t                       bufferCount
    };

    if( vkAllocateCommandBuffers( GetDevice(), &command_buffer_allocate_info, command_buffers ) != VK_SUCCESS ) {
      return false;
    }
    return true;
  }

  bool Tutorial03::RecordCommandBuffers() {
    VkCommandBufferBeginInfo graphics_commandd_buffer_begin_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
      nullptr,                                        // const void                            *pNext
      VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,   // VkCommandBufferUsageFlags              flags
      nullptr                                         // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    VkImageSubresourceRange image_subresource_range = {
      VK_IMAGE_ASPECT_COLOR_BIT,                      // VkImageAspectFlags             aspectMask
      0,                                              // uint32_t                       baseMipLevel
      1,                                              // uint32_t                       levelCount
      0,                                              // uint32_t                       baseArrayLayer
      1                                               // uint32_t                       layerCount
    };

    VkClearValue clear_value = {
      { {1.0f, 0.8f, 0.4f, 0.0f} },                     // VkClearColorValue              color
    };

    const std::vector<ImageParameters>& swap_chain_images = GetSwapChain().Images;

    for( size_t i = 0; i < Vulkan.GraphicsCommandBuffers.size(); ++i ) {
      vkBeginCommandBuffer( Vulkan.GraphicsCommandBuffers[i], &graphics_commandd_buffer_begin_info );

      if( GetPresentQueue().Handle != GetGraphicsQueue().Handle ) {
        VkImageMemoryBarrier barrier_from_present_to_draw = {
          VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,     // VkStructureType                sType
          nullptr,                                    // const void                    *pNext
          VK_ACCESS_MEMORY_READ_BIT,                  // VkAccessFlags                  srcAccessMask
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,       // VkAccessFlags                  dstAccessMask
          VK_IMAGE_LAYOUT_UNDEFINED,                  // VkImageLayout                  oldLayout
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,            // VkImageLayout                  newLayout
          GetPresentQueue().FamilyIndex,              // uint32_t                       srcQueueFamilyIndex
          GetGraphicsQueue().FamilyIndex,             // uint32_t                       dstQueueFamilyIndex
          swap_chain_images[i].Handle,                // VkImage                        image
          image_subresource_range                     // VkImageSubresourceRange        subresourceRange
        };
        vkCmdPipelineBarrier( Vulkan.GraphicsCommandBuffers[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_draw );
      }

      VkRenderPassBeginInfo render_pass_begin_info = {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,     // VkStructureType                sType
        nullptr,                                      // const void                    *pNext
        Vulkan.RenderPass,                            // VkRenderPass                   renderPass
        Vulkan.Framebuffers[i],                       // VkFramebuffer                  framebuffer
        {                                             // VkRect2D                       renderArea
          {                                           // VkOffset2D                     offset
            0,                                          // int32_t                        x
            0                                           // int32_t                        y
          },
          {                                           // VkExtent2D                     extent
            300,                                        // int32_t                        width
            300,                                        // int32_t                        height
          }
        },
        1,                                            // uint32_t                       clearValueCount
        &clear_value                                  // const VkClearValue            *pClearValues
      };

      vkCmdBeginRenderPass( Vulkan.GraphicsCommandBuffers[i], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );

      vkCmdBindPipeline( Vulkan.GraphicsCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan.GraphicsPipeline );

      vkCmdDraw( Vulkan.GraphicsCommandBuffers[i], 3, 1, 0, 0 );

      vkCmdEndRenderPass( Vulkan.GraphicsCommandBuffers[i] );

      if( GetGraphicsQueue().Handle != GetPresentQueue().Handle ) {
        VkImageMemoryBarrier barrier_from_draw_to_present = {
          VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,       // VkStructureType              sType
          nullptr,                                      // const void                  *pNext
          VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,         // VkAccessFlags                srcAccessMask
          VK_ACCESS_MEMORY_READ_BIT,                    // VkAccessFlags                dstAccessMask
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,              // VkImageLayout                oldLayout
          VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,              // VkImageLayout                newLayout
          GetGraphicsQueue().FamilyIndex,               // uint32_t                     srcQueueFamilyIndex
          GetPresentQueue().FamilyIndex,                // uint32_t                     dstQueueFamilyIndex
          swap_chain_images[i].Handle,                  // VkImage                      image
          image_subresource_range                       // VkImageSubresourceRange      subresourceRange
        };
        vkCmdPipelineBarrier( Vulkan.GraphicsCommandBuffers[i], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_draw_to_present );
      }
      if( vkEndCommandBuffer( Vulkan.GraphicsCommandBuffers[i] ) != VK_SUCCESS ) {
        std::cout << "Could not record command buffer!" << std::endl;
        return false;
      }
    }
    return true;
  }

  bool Tutorial03::ChildOnWindowSizeChanged() {
    if( !CreateRenderPass() ) {
      return false;
    }
    if( !CreateFramebuffers() ) {
      return false;
    }
    if( !CreatePipeline() ) {
      return false;
    }
    if( !CreateCommandBuffers() ) {
      return false;
    }
    if( !RecordCommandBuffers() ) {
      return false;
    }

    return true;
  }

  bool Tutorial03::Draw() {
    VkSwapchainKHR swap_chain = GetSwapChain().Handle;
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR( GetDevice(), swap_chain, UINT64_MAX, Vulkan.ImageAvailableSemaphore, VK_NULL_HANDLE, &image_index );
    switch( result ) {
      case VK_SUCCESS:
      case VK_SUBOPTIMAL_KHR:
        break;
      case VK_ERROR_OUT_OF_DATE_KHR:
        return OnWindowSizeChanged();
      default:
        std::cout << "Problem occurred during swap chain image acquisition!" << std::endl;
        return false;
    }

    VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info = {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      1,                                            // uint32_t                     waitSemaphoreCount
      &Vulkan.ImageAvailableSemaphore,              // const VkSemaphore           *pWaitSemaphores
      &wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
      1,                                            // uint32_t                     commandBufferCount
      &Vulkan.GraphicsCommandBuffers[image_index],  // const VkCommandBuffer       *pCommandBuffers
      1,                                            // uint32_t                     signalSemaphoreCount
      &Vulkan.RenderingFinishedSemaphore            // const VkSemaphore           *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_info, VK_NULL_HANDLE ) != VK_SUCCESS ) {
      return false;
    }

    VkPresentInfoKHR present_info = {
      VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      1,                                            // uint32_t                     waitSemaphoreCount
      &Vulkan.RenderingFinishedSemaphore,           // const VkSemaphore           *pWaitSemaphores
      1,                                            // uint32_t                     swapchainCount
      &swap_chain,                                  // const VkSwapchainKHR        *pSwapchains
      &image_index,                                 // const uint32_t              *pImageIndices
      nullptr                                       // VkResult                    *pResults
    };
    result = vkQueuePresentKHR( GetPresentQueue().Handle, &present_info );

    switch( result ) {
      case VK_SUCCESS:
        break;
      case VK_ERROR_OUT_OF_DATE_KHR:
      case VK_SUBOPTIMAL_KHR:
        return OnWindowSizeChanged();
      default:
        std::cout << "Problem occurred during image presentation!" << std::endl;
        return false;
    }

    return true;
  }

  void Tutorial03::ChildClear() {
    if( GetDevice() != VK_NULL_HANDLE ) {
      vkDeviceWaitIdle( GetDevice() );

      if( (Vulkan.GraphicsCommandBuffers.size() > 0) && (Vulkan.GraphicsCommandBuffers[0] != VK_NULL_HANDLE) ) {
        vkFreeCommandBuffers( GetDevice(), Vulkan.GraphicsCommandPool, static_cast<uint32_t>(Vulkan.GraphicsCommandBuffers.size()), &Vulkan.GraphicsCommandBuffers[0] );
        Vulkan.GraphicsCommandBuffers.clear();
      }

      if( Vulkan.GraphicsCommandPool != VK_NULL_HANDLE ) {
        vkDestroyCommandPool( GetDevice(), Vulkan.GraphicsCommandPool, nullptr );
        Vulkan.GraphicsCommandPool = VK_NULL_HANDLE;
      }

      if( Vulkan.GraphicsPipeline != VK_NULL_HANDLE ) {
        vkDestroyPipeline( GetDevice(), Vulkan.GraphicsPipeline, nullptr );
        Vulkan.GraphicsPipeline = VK_NULL_HANDLE;
      }

      if( Vulkan.RenderPass != VK_NULL_HANDLE ) {
        vkDestroyRenderPass( GetDevice(), Vulkan.RenderPass, nullptr );
        Vulkan.RenderPass = VK_NULL_HANDLE;
      }

      for( size_t i = 0; i < Vulkan.Framebuffers.size(); ++i ) {
        if( Vulkan.Framebuffers[i] != VK_NULL_HANDLE ) {
          vkDestroyFramebuffer( GetDevice(), Vulkan.Framebuffers[i], nullptr );
          Vulkan.Framebuffers[i] = VK_NULL_HANDLE;
        }
      }
      Vulkan.Framebuffers.clear();
    }
  }

  Tutorial03::~Tutorial03() {
    ChildClear();

    if( GetDevice() != VK_NULL_HANDLE ) {
      vkDeviceWaitIdle( GetDevice() );

      if( Vulkan.ImageAvailableSemaphore != VK_NULL_HANDLE ) {
        vkDestroySemaphore( GetDevice(), Vulkan.ImageAvailableSemaphore, nullptr );
      }

      if( Vulkan.RenderingFinishedSemaphore != VK_NULL_HANDLE ) {
        vkDestroySemaphore( GetDevice(), Vulkan.RenderingFinishedSemaphore, nullptr );
      }
    }
  }

} // namespace ApiWithoutSecrets
