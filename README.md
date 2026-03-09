# API without Secrets: Introduction to Vulkan
by Pawel Lapinski

> **Note:** The original tutorial pages hosted on Intel's website are no longer available, as Intel has discontinued this project and the original author no longer maintains it. The tutorial documents have been converted from the original PDF files to Markdown format and are preserved in the [`Document/`](./Document/) folder so that learners can still follow along with the written content alongside the source code examples.

## What is Vulkan?

Vulkan is a low-overhead, cross-platform graphics and compute API developed by the Khronos Group. Unlike older APIs such as OpenGL, Vulkan gives developers explicit control over GPU memory management, synchronization, and command submission. This design reduces CPU overhead and enables more predictable, high-performance rendering — making it well suited for games, simulations, and other demanding real-time applications.

Vulkan runs on Windows, Linux, Android, and other platforms that support a conformant driver. Hardware vendors publish drivers at https://www.khronos.org/vulkan/.

## About This Tutorial Series

"API without Secrets: Introduction to Vulkan" is a step-by-step tutorial series written by Pawel Lapinski that walks through the core concepts of the Vulkan API by building up a working renderer from scratch. Each part introduces new Vulkan objects and techniques, accompanied by complete, buildable source code examples.

Special thanks to Slawomir Cygan for help and for patiently answering many, many questions!

## Tutorials

### [Part 1 – The Beginning](./Document/01.md)
<img src="./Document/Images/01 - The Beginning.png" height="96px" align="right">

#### Introduction to a Vulkan world

Tutorial presents how to create all resources necessary to use Vulkan inside our application: function pointers loading, Vulkan instance creation, physical device enumeration, logical device creation and queue set up.

📂 [Source code](./Project/Tutorials/01/)

<hr>

### [Part 2 – Swap Chain](./Document/02.md)
<img src="./Document/Images/02 - Swap Chain.png" height="96px" align="right">

#### Integrating Vulkan with OS

This lesson focuses on a swap chain creation. Swap chain enables us to display Vulkan-generated image in an application window. To display anything simple command buffers are allocated and recorded.

📂 [Source code](./Project/Tutorials/02/)

<hr>

### [Part 3 – First Triangle](./Document/03.md)
<img src="./Document/Images/03 - First Triangle.png" height="96px" align="right">

#### Graphics pipeline and drawing

Here I present render pass, framebuffer and pipeline objects which are necessary to render arbitrary geometry. It is also shown how to convert GLSL shaders into SPIR-V and create shader modules from it.

📂 [Source code](./Project/Tutorials/03/)

<hr>

### [Part 4 – Vertex Attributes](./Document/04.md)
<img src="./Document/Images/04 - Vertex Attributes.png" height="96px" align="right">

#### Buffers, memory objects and fences

This tutorial shows how to set up vertex attributes and bind buffer with a vertex data. Here we also create memory object (which is used by buffer) and fences.

📂 [Source code](./Project/Tutorials/04/)

<hr>

### [Part 5 – Staging Resources](./Document/05.md)
<img src="./Document/Images/05 - Staging Resources.png" height="96px" align="right">

#### Copying data between buffers

In this example staging resources are presented. They are used as an intermediate resources for copying data between CPU and GPU. This way, resources involved in rendering can be bound only to a device local (very fast) memory.

📂 [Source code](./Project/Tutorials/05/)

<hr>

### [Part 6 – Descriptor Sets](./Document/06.md)
<img src="./Document/Images/06 - Descriptor Sets.png" height="96px" align="right">

#### Using textures in shaders

This tutorial shows what resources are needed and how they should be prepared to be able to use textures (or other shader resources) in shader programs.

📂 [Source code](./Project/Tutorials/06/)

<hr>

### [Part 7 – Uniform Buffers](./Document/07.md)
<img src="./Document/Images/07 - Uniform Buffers.png" height="96px" align="right">

#### Using buffers in shaders

Here it is shown how to add uniform buffer to descriptor sets, how to provide data for projection matrix through it and how to use it inside shader.

📂 [Source code](./Project/Tutorials/07/)

<hr>

## Original Project Discontinuation Notice

This project was originally developed and maintained by Intel. Intel has ceased development and contributions including, but not limited to, maintenance, bug fixes, new releases, or updates, to this project. Intel no longer accepts patches to this project.

If you have an ongoing need to use this project, are interested in independently developing it, or would like to maintain patches for the open source software community, please create your own fork of this project.
