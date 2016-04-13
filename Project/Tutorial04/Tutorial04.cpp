// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#include "Tutorial04.h"
#include "VulkanFunctions.h"

namespace ApiWithoutSecrets {

  Tutorial04::Tutorial04() :
    Vulkan() {
  }

  bool Tutorial04::CreateRenderPass() {
    VkAttachmentDescription attachment_descriptions[] = {
      {
        0,                                          // VkAttachmentDescriptionFlags   flags
        Vulkan.Image.Format,                        // VkFormat                       format
        VK_SAMPLE_COUNT_1_BIT,                      // VkSampleCountFlagBits          samples
        VK_ATTACHMENT_LOAD_OP_CLEAR,                // VkAttachmentLoadOp             loadOp
        VK_ATTACHMENT_STORE_OP_STORE,               // VkAttachmentStoreOp            storeOp
        VK_ATTACHMENT_LOAD_OP_DONT_CARE,            // VkAttachmentLoadOp             stencilLoadOp
        VK_ATTACHMENT_STORE_OP_DONT_CARE,           // VkAttachmentStoreOp            stencilStoreOp
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,       // VkImageLayout                  initialLayout;
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL        // VkImageLayout                  finalLayout
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

  bool Tutorial04::CreateImage() {
    VkImageCreateInfo image_create_info = {
      VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,          //  VkStructureType           sType
      nullptr,                                      // const void                *pNext
      0,                                            // VkImageCreateFlags         flags
      VK_IMAGE_TYPE_2D,                             // VkImageType                imageType
      Vulkan.Image.Format,                          // VkFormat                   format
      {                                             // VkExtent3D                 extent
        300,                                          // uint32_t                   width
        300,                                          // uint32_t                   height
        1                                             // uint32_t                   depth
      },
      1,                                            // uint32_t                   mipLevels
      1,                                            // uint32_t                   arrayLayers
      VK_SAMPLE_COUNT_1_BIT,                        // VkSampleCountFlagBits      samples
      VK_IMAGE_TILING_OPTIMAL,                      // VkImageTiling              tiling
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT |             // VkImageUsageFlags          usage
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      VK_SHARING_MODE_EXCLUSIVE,                    // VkSharingMode              sharingMode
      0,                                            // uint32_t                   queueFamilyIndexCount
      nullptr,                                      // const uint32_t            *pQueueFamilyIndices
      VK_IMAGE_LAYOUT_UNDEFINED                     // VkImageLayout              initialLayout
    };

    if( vkCreateImage( GetDevice(), &image_create_info, nullptr, &Vulkan.Image.Handle ) != VK_SUCCESS ) {
      std::cout << "Could not create an image!" << std::endl;
      return false;
    }

    if( !AllocateImageMemory( Vulkan.Image.Handle, &Vulkan.Image.Memory ) ) {
      std::cout << "Could not allocate memory for an image!" << std::endl;
      return false;
    }

    if( vkBindImageMemory( GetDevice(), Vulkan.Image.Handle, Vulkan.Image.Memory, 0 ) != VK_SUCCESS ) {
      std::cout << "Could not bind memory for an image!" << std::endl;
      return false;
    }

    return true;
  }

  bool Tutorial04::AllocateImageMemory( VkImage image, VkDeviceMemory *memory ) {
    VkMemoryRequirements image_memory_requirements;
    vkGetImageMemoryRequirements( GetDevice(), Vulkan.Image.Handle, &image_memory_requirements );

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties( GetPhysicalDevice(), &memory_properties );

    for( uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i ) {
      if( image_memory_requirements.memoryTypeBits & (1 << i) ) {

        VkMemoryAllocateInfo memory_allocate_info = {
          VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,   // VkStructureType            sType
          nullptr,                                  // const void                *pNext
          image_memory_requirements.size,           // VkDeviceSize               allocationSize
          i                                         // uint32_t                   memoryTypeIndex
        };

        if( vkAllocateMemory( GetDevice(), &memory_allocate_info, nullptr, memory ) == VK_SUCCESS ) {
          return true;
        }
      }
    }
    return false;
  }

  bool Tutorial04::CreateFramebuffer() {
    VkImageViewCreateInfo image_view_create_info = {
      VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,   // VkStructureType                sType
      nullptr,                                    // const void                    *pNext
      0,                                          // VkImageViewCreateFlags         flags
      Vulkan.Image.Handle,                        // VkImage                        image
      VK_IMAGE_VIEW_TYPE_2D,                      // VkImageViewType                viewType
      Vulkan.Image.Format,                        // VkFormat                       format
      {                                           // VkComponentMapping             components
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             r
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             g
        VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle             b
        VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle             a
      },
      {                                           // VkImageSubresourceRange        subresourceRange
        VK_IMAGE_ASPECT_COLOR_BIT,                  // VkImageAspectFlags             aspectMask
        0,                                          // uint32_t                       baseMipLevel
        1,                                          // uint32_t                       levelCount
        0,                                          // uint32_t                       baseArrayLayer
        1                                           // uint32_t                       layerCount
      }
    };

    if( vkCreateImageView( GetDevice(), &image_view_create_info, nullptr, &Vulkan.Image.View ) != VK_SUCCESS ) {
      std::cout << "Could not create an image view!" << std::endl;
      return false;
    }

    VkFramebufferCreateInfo framebuffer_create_info = {
      VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,  // VkStructureType                sType
      nullptr,                                    // const void                    *pNext
      0,                                          // VkFramebufferCreateFlags       flags
      Vulkan.RenderPass,                          // VkRenderPass                   renderPass
      1,                                          // uint32_t                       attachmentCount
      &Vulkan.Image.View,                         // const VkImageView             *pAttachments
      300,                                        // uint32_t                       width
      300,                                        // uint32_t                       height
      1                                           // uint32_t                       layers
    };

    if( vkCreateFramebuffer( GetDevice(), &framebuffer_create_info, nullptr, &Vulkan.Framebuffer ) != VK_SUCCESS ) {
      std::cout << "Could not create a framebuffer!" << std::endl;
      return false;
    }
    return true;
  }

  Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> Tutorial04::CreateShaderModule( const char* filename ) {
    const std::vector<char> code = Tools::GetBinaryFileContents( filename );
    if( code.size() == 0 ) {
      return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();
    }

    VkShaderModuleCreateInfo shader_module_create_info = {
      VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,    // VkStructureType                sType
      nullptr,                                        // const void                    *pNext
      0,                                              // VkShaderModuleCreateFlags      flags
      code.size(),                                    // size_t                         codeSize
      reinterpret_cast<const uint32_t*>(&code[0])     // const uint32_t                *pCode
    };

    VkShaderModule shader_module;
    if( vkCreateShaderModule( GetDevice(), &shader_module_create_info, nullptr, &shader_module ) != VK_SUCCESS ) {
      std::cout << "Could not create shader module from a \"" << filename << "\" file!" << std::endl;
      return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>();
    }

    return Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule>( shader_module, vkDestroyShaderModule, GetDevice() );
  }

  Tools::AutoDeleter<VkPipelineLayout, PFN_vkDestroyPipelineLayout> Tutorial04::CreatePipelineLayout() {
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

  bool Tutorial04::CreatePipeline() {
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> vertex_shader_module = CreateShaderModule( "Data04/vert.spv" );
    Tools::AutoDeleter<VkShaderModule, PFN_vkDestroyShaderModule> fragment_shader_module = CreateShaderModule( "Data04/frag.spv" );

    if( !vertex_shader_module || !fragment_shader_module ) {
      return false;
    }

    std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos = {
      // Vertex shader
      {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
        nullptr,                                                    // const void                                    *pNext
        0,                                                          // VkPipelineShaderStageCreateFlags               flags
        VK_SHADER_STAGE_VERTEX_BIT,                                 // VkShaderStageFlagBits                          stage
        vertex_shader_module.Get(),                                 // VkShaderModule                                 module
        "main",                                                     // const char                                    *pName
        nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
      },
      // Fragment shader
      {
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,        // VkStructureType                                sType
        nullptr,                                                    // const void                                    *pNext
        0,                                                          // VkPipelineShaderStageCreateFlags               flags
        VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlagBits                          stage
        fragment_shader_module.Get(),                               // VkShaderModule                                 module
        "main",                                                     // const char                                    *pName
        nullptr                                                     // const VkSpecializationInfo                    *pSpecializationInfo
      }
    };

    VkVertexInputBindingDescription vertex_binding_description = {
      0,                                                            // uint32_t                                       binding
      sizeof(VertexData),                                           // uint32_t                                       stride
      VK_VERTEX_INPUT_RATE_VERTEX                                   // VkVertexInputRate                              inputRate
    };

    VkVertexInputAttributeDescription vertex_attribute_descriptions[] = {
      {
        0,                                                          // uint32_t                                       location
        vertex_binding_description.binding,                         // uint32_t                                       binding
        VK_FORMAT_R32G32B32A32_SFLOAT,                              // VkFormat                                       format
        0                                                           // uint32_t                                       offset
      },
      {
        1,                                                          // uint32_t                                       location
        vertex_binding_description.binding,                         // uint32_t                                       binding
        VK_FORMAT_R32G32B32A32_SFLOAT,                              // VkFormat                                       format
        4 * sizeof(float)                                           // uint32_t                                       offset
      }
    };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,    // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineVertexInputStateCreateFlags          flags;
      1,                                                            // uint32_t                                       vertexBindingDescriptionCount
      &vertex_binding_description,                                  // const VkVertexInputBindingDescription         *pVertexBindingDescriptions
      2,                                                            // uint32_t                                       vertexAttributeDescriptionCount
      vertex_attribute_descriptions                                 // const VkVertexInputAttributeDescription       *pVertexAttributeDescriptions
    };

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,  // VkStructureType                                sType
      nullptr,                                                      // const void                                    *pNext
      0,                                                            // VkPipelineInputAssemblyStateCreateFlags        flags
      VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,                         // VkPrimitiveTopology                            topology
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
    return true;
  }

  bool Tutorial04::CreateVertexBuffer() {
    VertexData vertex_data[] = {
      {
        -0.7f, -0.7f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f
      },
      {
        -0.7f, 0.7f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 0.0f
      },
      {
        0.7f, -0.7f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f
      },
      {
        0.7f, 0.7f, 0.0f, 1.0f,
        0.3f, 0.3f, 0.3f, 0.0f
      }
    };

    Vulkan.VertexBuffer.Size = sizeof(vertex_data);

    VkBufferCreateInfo buffer_create_info = {
      VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,             // VkStructureType                    sType
      nullptr,                                          // const void                        *pNext
      0,                                                // VkBufferCreateFlags                flags
      Vulkan.VertexBuffer.Size,                         // VkDeviceSize                       size
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,                // VkBufferUsageFlags                 usage
      VK_SHARING_MODE_EXCLUSIVE,                        // VkSharingMode                      sharingMode
      0,                                                // uint32_t                           queueFamilyIndexCount
      nullptr                                           // const uint32_t                    *pQueueFamilyIndices
    };

    if( vkCreateBuffer( GetDevice(), &buffer_create_info, nullptr, &Vulkan.VertexBuffer.Handle ) != VK_SUCCESS ) {
      std::cout << "Could not create a vertex buffer!" << std::endl;
      return false;
    }

    if( !AllocateBufferMemory( Vulkan.VertexBuffer.Handle, &Vulkan.VertexBuffer.Memory ) ) {
      std::cout << "Could not allocate memory for a vertex buffer!" << std::endl;
      return false;
    }

    if( vkBindBufferMemory( GetDevice(), Vulkan.VertexBuffer.Handle, Vulkan.VertexBuffer.Memory, 0 ) != VK_SUCCESS ) {
      std::cout << "Could not bind memory for a vertex buffer!" << std::endl;
      return false;
    }

    void *vertex_buffer_memory_pointer;
    if( vkMapMemory( GetDevice(), Vulkan.VertexBuffer.Memory, 0, Vulkan.VertexBuffer.Size, 0, &vertex_buffer_memory_pointer ) != VK_SUCCESS ) {
      std::cout << "Could not map memory and upload data to a vertex buffer!" << std::endl;
      return false;
    }

    memcpy( vertex_buffer_memory_pointer, vertex_data, Vulkan.VertexBuffer.Size );

    vkUnmapMemory( GetDevice(), Vulkan.VertexBuffer.Memory );

    if( !CommitMemoryChanges( Vulkan.VertexBuffer.Handle, Vulkan.VertexBuffer.Size ) ) {
      std::cout << "Could not setup a barrier for a vertex buffer!" << std::endl;
      return false;
    }

    return true;
  }

  bool Tutorial04::AllocateBufferMemory( VkBuffer buffer, VkDeviceMemory *memory ) {
    VkMemoryRequirements buffer_memory_requirements;
    vkGetBufferMemoryRequirements( GetDevice(), Vulkan.VertexBuffer.Handle, &buffer_memory_requirements );

    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties( GetPhysicalDevice(), &memory_properties );

    for( uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i ) {
      if( (buffer_memory_requirements.memoryTypeBits & (1 << i)) &&
          (memory_properties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ) {

        VkMemoryAllocateInfo memory_allocate_info = {
          VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,       // VkStructureType                    sType
          nullptr,                                      // const void                        *pNext
          buffer_memory_requirements.size,              // VkDeviceSize                       allocationSize
          i                                             // uint32_t                           memoryTypeIndex
        };

        if( vkAllocateMemory( GetDevice(), &memory_allocate_info, nullptr, memory ) == VK_SUCCESS ) {
          return true;
        }
      }
    }
    return false;
  }

  bool Tutorial04::CommitMemoryChanges( VkBuffer buffer, VkDeviceSize size ) {
    VkCommandBufferBeginInfo command_buffer_begin_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
      nullptr,                                        // const void                            *pNext
      VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,    // VkCommandBufferUsageFlags              flags
      nullptr                                         // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    VkBufferMemoryBarrier barrier_from_host_write_to_attribute_read = {
      VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,        // VkStructureType                      sType
      nullptr,                                        // const void                          *pNext
      VK_ACCESS_HOST_WRITE_BIT,                       // VkAccessFlags                        srcAccessMask
      VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,            // VkAccessFlags                        dstAccessMask
      GetGraphicsQueue().FamilyIndex,                 // uint32_t                             srcQueueFamilyIndex
      GetGraphicsQueue().FamilyIndex,                 // uint32_t                             dstQueueFamilyIndex
      buffer,                                         // VkBuffer                             buffer
      0,                                              // VkDeviceSize                         offset
      size                                            // VkDeviceSize                         size
    };

    vkBeginCommandBuffer( Vulkan.CopyingCommandBuffer, &command_buffer_begin_info );

    vkCmdPipelineBarrier( Vulkan.CopyingCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 1, &barrier_from_host_write_to_attribute_read, 0, nullptr );

    if( vkEndCommandBuffer( Vulkan.CopyingCommandBuffer ) != VK_SUCCESS ) {
      std::cout << "Could not record command with buffer barrier!" << std::endl;
      return false;
    }

    VkSubmitInfo submit_rendering_info = {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      0,                                            // uint32_t                     waitSemaphoreCount
      nullptr,                                      // const VkSemaphore           *pWaitSemaphores
      nullptr,                                      // const VkPipelineStageFlags  *pWaitDstStageMask;
      1,                                            // uint32_t                     commandBufferCount
      &Vulkan.CopyingCommandBuffer,                 // const VkCommandBuffer       *pCommandBuffers
      0,                                            // uint32_t                     signalSemaphoreCount
      nullptr                                       // const VkSemaphore           *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_rendering_info, VK_NULL_HANDLE ) != VK_SUCCESS ) {
      std::cout << "Error occurred during submission of command buffer with vertex buffer barrier!!" << std::endl;
      return false;
    }

    vkDeviceWaitIdle( GetDevice() );

    return true;
  }

  bool Tutorial04::CreateFence() {
    VkFenceCreateInfo fence_create_info = {
      VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,          // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      VK_FENCE_CREATE_SIGNALED_BIT                  // VkFenceCreateFlags           flags
    };
    if( vkCreateFence( GetDevice(), &fence_create_info, nullptr, &Vulkan.Fence ) != VK_SUCCESS ) {
      std::cout << "Could not create a fence!" << std::endl;
      return false;
    }
    return true;
  }

  bool Tutorial04::CreateCommandBuffers() {
    if( !CreateCommandPool( GetGraphicsQueue().FamilyIndex, &Vulkan.GraphicsCommandPool ) ) {
      std::cout << "Could not create command pool!" << std::endl;
      return false;
    }
    if( !AllocateCommandBuffers( Vulkan.GraphicsCommandPool, 1, &Vulkan.RenderingCommandBuffer ) ) {
      std::cout << "Could not allocate rendering command buffer!" << std::endl;
      return false;
    }
    if( !AllocateCommandBuffers( Vulkan.GraphicsCommandPool, 1, &Vulkan.CopyingCommandBuffer ) ) {
      std::cout << "Could not allocate copying command buffer!" << std::endl;
      return false;
    }
    return true;
  }

  bool Tutorial04::CreateCommandPool( uint32_t queue_family_index, VkCommandPool *pool ) {
    VkCommandPoolCreateInfo cmd_pool_create_info = {
      VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,       // VkStructureType                sType
      nullptr,                                          // const void                    *pNext
      VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,  // VkCommandPoolCreateFlags       flags
      queue_family_index                                // uint32_t                       queueFamilyIndex
    };

    if( vkCreateCommandPool( GetDevice(), &cmd_pool_create_info, nullptr, pool ) != VK_SUCCESS ) {
      return false;
    }
    return true;
  }

  bool Tutorial04::AllocateCommandBuffers( VkCommandPool pool, uint32_t count, VkCommandBuffer *command_buffers ) {
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,   // VkStructureType                sType
      nullptr,                                          // const void                    *pNext
      pool,                                             // VkCommandPool                  commandPool
      VK_COMMAND_BUFFER_LEVEL_PRIMARY,                  // VkCommandBufferLevel           level
      count                                             // uint32_t                       bufferCount
    };

    if( vkAllocateCommandBuffers( GetDevice(), &command_buffer_allocate_info, command_buffers ) != VK_SUCCESS ) {
      return false;
    }
    return true;
  }

  bool Tutorial04::RecordRenderingCommandBuffer() {
    VkCommandBufferBeginInfo command_buffer_begin_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
      nullptr,                                        // const void                            *pNext
      0,                                              // VkCommandBufferUsageFlags              flags
      nullptr                                         // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    vkBeginCommandBuffer( Vulkan.RenderingCommandBuffer, &command_buffer_begin_info );

    VkClearValue clear_value = {
      { 1.0f, 0.8f, 0.4f, 0.0f },                     // VkClearColorValue              color
    };

    VkRenderPassBeginInfo render_pass_begin_info = {
      VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,       // VkStructureType                sType
      nullptr,                                        // const void                    *pNext
      Vulkan.RenderPass,                              // VkRenderPass                   renderPass
      Vulkan.Framebuffer,                             // VkFramebuffer                  framebuffer
      {                                               // VkRect2D                       renderArea
        {                                             // VkOffset2D                     offset
          0,                                            // int32_t                        x
          0                                             // int32_t                        y
        },
        {                                             // VkExtent2D                     extent
          300,                                          // int32_t                        width
          300,                                          // int32_t                        height
        }
      },
      1,                                              // uint32_t                       clearValueCount
      &clear_value                                    // const VkClearValue            *pClearValues
    };

    vkCmdBeginRenderPass( Vulkan.RenderingCommandBuffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( Vulkan.RenderingCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, Vulkan.GraphicsPipeline );

    VkDeviceSize offset = 0;
    vkCmdBindVertexBuffers( Vulkan.RenderingCommandBuffer, 0, 1, &Vulkan.VertexBuffer.Handle, &offset );

    vkCmdDraw( Vulkan.RenderingCommandBuffer, 4, 1, 0, 0 );

    vkCmdEndRenderPass( Vulkan.RenderingCommandBuffer );

    if( vkEndCommandBuffer( Vulkan.RenderingCommandBuffer ) != VK_SUCCESS ) {
      std::cout << "Could not record drawing command buffer!" << std::endl;
      return false;
    }
    return true;
  }

  bool Tutorial04::RecordCopyingCommandBuffer( VkImage swap_chain_image ) {
    if( vkWaitForFences( GetDevice(), 1, &Vulkan.Fence, VK_FALSE, 1000000000 ) != VK_SUCCESS ) {
      std::cout << "Waiting for fence takes too long!" << std::endl;
      return false;
    }
    vkResetFences( GetDevice(), 1, &Vulkan.Fence );

    VkCommandBufferBeginInfo command_buffer_begin_info = {
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,    // VkStructureType                        sType
      nullptr,                                        // const void                            *pNext
      VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,    // VkCommandBufferUsageFlags              flags
      nullptr                                         // const VkCommandBufferInheritanceInfo  *pInheritanceInfo
    };

    vkBeginCommandBuffer( Vulkan.CopyingCommandBuffer, &command_buffer_begin_info );

    VkImageSubresourceRange image_subresource_range = {
      VK_IMAGE_ASPECT_COLOR_BIT,                      // VkImageAspectFlags           aspectMask
      0,                                              // uint32_t                     baseMipLevel
      1,                                              // uint32_t                     levelCount
      0,                                              // uint32_t                     baseArrayLayer
      1                                               // uint32_t                     layerCount
    };

    VkImageMemoryBarrier barrier_from_present_to_transfer_dst = {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,         // VkStructureType              sType
      nullptr,                                        // const void                  *pNext
      VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags                srcAccessMask
      VK_ACCESS_TRANSFER_WRITE_BIT,                   // VkAccessFlags                dstAccessMask
      VK_IMAGE_LAYOUT_UNDEFINED,                      // VkImageLayout                oldLayout
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,           // VkImageLayout                newLayout
      GetPresentQueue().FamilyIndex,                  // uint32_t                     srcQueueFamilyIndex
      GetGraphicsQueue().FamilyIndex,                 // uint32_t                     dstQueueFamilyIndex
      swap_chain_image,                               // VkImage                      image
      image_subresource_range                         // VkImageSubresourceRange      subresourceRange
    };

    vkCmdPipelineBarrier( Vulkan.CopyingCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_present_to_transfer_dst );

    VkImageCopy image_copy = {
      {                                               // VkImageSubresourceLayers     srcSubresource
        VK_IMAGE_ASPECT_COLOR_BIT,                      // VkImageAspectFlags           aspectMask
        0,                                              // uint32_t                     mipLevel
        0,                                              // uint32_t                     baseArrayLayer
        1                                               // uint32_t                     layerCount
      },
      {                                               // VkOffset3D                   srcOffset
        0,                                              // int32_t                      x
        0,                                              // int32_t                      y
        0                                               // int32_t                      z
      },
      {                                               // VkImageSubresourceLayers     dstSubresource
        VK_IMAGE_ASPECT_COLOR_BIT,                      // VkImageAspectFlags           aspectMask
        0,                                              // uint32_t                     mipLevel
        0,                                              // uint32_t                     baseArrayLayer
        1                                               // uint32_t                     layerCount
      },
      {                                               // VkOffset3D                   dstOffset
        0,                                              // int32_t                      x
        0,                                              // int32_t                      y
        0                                               // int32_t                      z
      },
      {                                               // VkExtent3D                   extent
        300,                                            // uint32_t                     width
        300,                                            // uint32_t                     height
        1                                               // uint32_t                     depth
      }
    };

    vkCmdCopyImage( Vulkan.CopyingCommandBuffer, Vulkan.Image.Handle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, swap_chain_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &image_copy );

    VkImageMemoryBarrier barrier_from_transfer_dst_to_present = {
      VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,         // VkStructureType                        sType
      nullptr,                                        // const void                            *pNext
      VK_ACCESS_TRANSFER_WRITE_BIT,                   // VkAccessFlags                          srcAccessMask
      VK_ACCESS_MEMORY_READ_BIT,                      // VkAccessFlags                          dstAccessMask
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,           // VkImageLayout                          oldLayout
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,                // VkImageLayout                          newLayout
      GetGraphicsQueue().FamilyIndex,                 // uint32_t                               srcQueueFamilyIndex
      GetPresentQueue().FamilyIndex,                  // uint32_t                               dstQueueFamilyIndex
      swap_chain_image,                               // VkImage                                image
      image_subresource_range                         // VkImageSubresourceRange                subresourceRange
    };

    vkCmdPipelineBarrier( Vulkan.CopyingCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier_from_transfer_dst_to_present );

    if( vkEndCommandBuffer( Vulkan.CopyingCommandBuffer ) != VK_SUCCESS ) {
      std::cout << "Could not record copying command buffer!" << std::endl;
      return false;
    }
    return true;
  }

  bool Tutorial04::ChildOnWindowSizeChanged() {
    return true;
  }

  bool Tutorial04::Draw() {
    VkSemaphore image_available_semaphore = GetImageAvailableSemaphore();
    VkSemaphore rendering_finished_semaphore = GetRenderingFinishedSemaphore();
    VkSwapchainKHR swap_chain = GetSwapChain().Handle;
    uint32_t image_index;

    VkResult result = vkAcquireNextImageKHR( GetDevice(), swap_chain, UINT64_MAX, image_available_semaphore, VK_NULL_HANDLE, &image_index );
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

    VkSubmitInfo submit_rendering_info = {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      0,                                            // uint32_t                     waitSemaphoreCount
      nullptr,                                      // const VkSemaphore           *pWaitSemaphores
      nullptr,                                      // const VkPipelineStageFlags  *pWaitDstStageMask;
      1,                                            // uint32_t                     commandBufferCount
      &Vulkan.RenderingCommandBuffer,               // const VkCommandBuffer       *pCommandBuffers
      0,                                            // uint32_t                     signalSemaphoreCount
      nullptr                                       // const VkSemaphore           *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_rendering_info, VK_NULL_HANDLE ) != VK_SUCCESS ) {
      return false;
    }

    if( !RecordCopyingCommandBuffer( GetSwapChain().Images[image_index] ) ) {
      return false;
    }

    VkPipelineStageFlags wait_dst_stage_mask = VK_PIPELINE_STAGE_TRANSFER_BIT;
    VkSubmitInfo submit_copying_info = {
      VK_STRUCTURE_TYPE_SUBMIT_INFO,                // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      1,                                            // uint32_t                     waitSemaphoreCount
      &image_available_semaphore,                   // const VkSemaphore           *pWaitSemaphores
      &wait_dst_stage_mask,                         // const VkPipelineStageFlags  *pWaitDstStageMask;
      1,                                            // uint32_t                     commandBufferCount
      &Vulkan.CopyingCommandBuffer,                 // const VkCommandBuffer       *pCommandBuffers
      1,                                            // uint32_t                     signalSemaphoreCount
      &rendering_finished_semaphore                 // const VkSemaphore           *pSignalSemaphores
    };

    if( vkQueueSubmit( GetGraphicsQueue().Handle, 1, &submit_copying_info, Vulkan.Fence ) != VK_SUCCESS ) {
      return false;
    }

    VkPresentInfoKHR present_info = {
      VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,           // VkStructureType              sType
      nullptr,                                      // const void                  *pNext
      1,                                            // uint32_t                     waitSemaphoreCount
      &rendering_finished_semaphore,                // const VkSemaphore           *pWaitSemaphores
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

  void Tutorial04::ChildClear() {
  }

  Tutorial04::~Tutorial04() {
    if( GetDevice() != VK_NULL_HANDLE ) {
      vkDeviceWaitIdle( GetDevice() );

      if( Vulkan.RenderingCommandBuffer != VK_NULL_HANDLE ) {
        vkFreeCommandBuffers( GetDevice(), Vulkan.GraphicsCommandPool, 1, &Vulkan.RenderingCommandBuffer );
        Vulkan.RenderingCommandBuffer = VK_NULL_HANDLE;
      }

      if( Vulkan.CopyingCommandBuffer != VK_NULL_HANDLE ) {
        vkFreeCommandBuffers( GetDevice(), Vulkan.GraphicsCommandPool, 1, &Vulkan.CopyingCommandBuffer );
        Vulkan.CopyingCommandBuffer = VK_NULL_HANDLE;
      }

      if( Vulkan.GraphicsCommandPool != VK_NULL_HANDLE ) {
        vkDestroyCommandPool( GetDevice(), Vulkan.GraphicsCommandPool, nullptr );
        Vulkan.GraphicsCommandPool = VK_NULL_HANDLE;
      }

      if( Vulkan.Fence != VK_NULL_HANDLE ) {
        vkDestroyFence( GetDevice(), Vulkan.Fence, nullptr );
        Vulkan.Fence = VK_NULL_HANDLE;
      }

      if( Vulkan.VertexBuffer.Handle != VK_NULL_HANDLE ) {
        vkDestroyBuffer( GetDevice(), Vulkan.VertexBuffer.Handle, nullptr );
        Vulkan.VertexBuffer.Handle = VK_NULL_HANDLE;
      }

      if( Vulkan.VertexBuffer.Memory != VK_NULL_HANDLE ) {
        vkFreeMemory( GetDevice(), Vulkan.VertexBuffer.Memory, nullptr );
        Vulkan.VertexBuffer.Memory = VK_NULL_HANDLE;
      }

      if( Vulkan.GraphicsPipeline != VK_NULL_HANDLE ) {
        vkDestroyPipeline( GetDevice(), Vulkan.GraphicsPipeline, nullptr );
        Vulkan.GraphicsPipeline = VK_NULL_HANDLE;
      }

      if( Vulkan.Framebuffer != VK_NULL_HANDLE ) {
        vkDestroyFramebuffer( GetDevice(), Vulkan.Framebuffer, nullptr );
        Vulkan.Framebuffer = VK_NULL_HANDLE;
      }

      if( Vulkan.Image.View != VK_NULL_HANDLE ) {
        vkDestroyImageView( GetDevice(), Vulkan.Image.View, nullptr );
        Vulkan.Image.View = VK_NULL_HANDLE;
      }

      if( Vulkan.Image.Handle != VK_NULL_HANDLE ) {
        vkDestroyImage( GetDevice(), Vulkan.Image.Handle, nullptr );
        Vulkan.Image.Handle = VK_NULL_HANDLE;
      }

      if( Vulkan.Image.Memory != VK_NULL_HANDLE ) {
        vkFreeMemory( GetDevice(), Vulkan.Image.Memory, nullptr );
        Vulkan.Image.Memory = VK_NULL_HANDLE;
      }

      if( Vulkan.RenderPass != VK_NULL_HANDLE ) {
        vkDestroyRenderPass( GetDevice(), Vulkan.RenderPass, nullptr );
        Vulkan.RenderPass = VK_NULL_HANDLE;
      }
    }
  }

} // namespace ApiWithoutSecrets