# API without Secrets: Introduction to Vulkan
by Pawel Lapinski

Source code examples for "API without Secrets: Introduction to Vulkan" tutorial which can be found at:

https://software.intel.com/en-us/articles/api-without-secrets-introduction-to-vulkan-preface

Special thanks to Slawomir Cygan for help and for patiently answering my many, many questions!

## Drivers:

Vulkan drivers and other related resources can be found at https://www.khronos.org/vulkan/

## Tutorials:

### [01 - The Beginning](./Project/Tutorial01/)
<img src="./Document/Images/01 - The Beginning.png" height="96px" align="right">

#### Introduction to a Vulkan world
##### https://software.intel.com/en-us/api-without-secrets-introduction-to-vulkan-part-1
Tutorial presents how to create all resources necessary to use Vulkan inside our application: function pointers loading, Vulkan instance creation, physical device enumeration, logical device creation and queue set up.

### [02 - Swap chain](./Project/Tutorial02/)
<img src="./Document/Images/02 - Swap Chain.png" height="96px" align="right">

#### Integrating Vulkan with OS
##### https://software.intel.com/en-us/api-without-secrets-introduction-to-vulkan-part-2
This lesson focuses on a swap chain creation. Swap chain enables us to display Vulkan-generated image in an application window. To display anything simple command buffers are allocated and recorded.

### [03 - First triangle](./Project/Tutorial03/)
<img src="./Document/Images/03 - First Triangle.png" height="96px" align="right">

#### Graphics pipeline and drawing
Here I present render pass, framebuffer and pipeline objects which are necessary to render arbitrary geometry. It is also shown how to convert GLSL shaders into SPIR-V and create shader modules from it.

### [04 - Vertex Attributes](./Project/Tutorial04/)
<img src="./Document/Images/04 - Vertex Attributes.png" height="96px" align="right">

#### Buffers, images and fences
This tutorial shows how to set up vertex attributes and bind buffer with vertex data. Here we also create images, memory objects and fences.
