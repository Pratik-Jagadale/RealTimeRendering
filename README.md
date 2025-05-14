# 🚀 Real-Time Rendering Projects Showcase

Welcome to the **RealTimeRendering** repository!  
This is a curated collection of projects developed as part of my **Real-Time Rendering (RTR)** course at AstroMediComp, showcasing graphics programming, cross-platform rendering, compute interoperability, and advanced GPU techniques.

![Rendering Animation]([https://media.giphy.com/media/LmNwrBhejkK9EFP504/giphy.gif)

---

## 🎯 Project Highlights

### 🧱 Core Topics Covered
- 🔺 **OpenGL** — Forward & Post-Processing Pipelines  
- 🎮 **DirectX11** — Real-time 3D rendering on Windows  
- ⚡ **HPP (High-Performance Parallelism)** using **CUDA & OpenCL**  
- 🔄 **CUDA–OpenGL Interop** — Zero-copy data sharing for simulation/visualization  
- 🧠 **Data Structures for Real-Time Rendering**  
- 🌐 **Cross-Platform Builds**: Windows, Linux, MacOS, Android, iOS, WebAssembly

---

## 🌍 Cross-Platform Support

| Platform    | Status      | Tech Stack |
|-------------|-------------|------------|
| 🪟 Windows   | ✅ Complete  | OpenGL, WebGL, DirectX11, CUDA |
| 🐧 Linux     | ✅ Complete  | OpenGL, CUDA, OpenCL |
| 🍏 macOS     | ✅ Complete  | OpenGL |
| 🤖 Android   | ✅ Complete  | OpenGL ES               |
| 🌐 Web       | ✅ Complete  | WebGL  |
| 📱 iOS       | ✅ Complete  | OpenGLES |

---

## 🧪 Project Categories

### 🔷 OpenGL & DirectX 11 Projects

- `PP_OpenGL/` - `DirectX11/` —

| 📂 Folder Name                       | 📝 Description                                                           |
|-------------------------------------|---------------------------------------------------------------------------|
| `00_BlueWindowWithDepth`            | Initializes OpenGL context with depth buffer                             |
| `01_OpenGLExtension`                | Sets up OpenGL extensions                                                |
| `02_BlueScreenWithPassThroughShaders` | Basic pass-through shader rendering                                   |
| `03_Ortho`                          | Demonstrates orthographic projection                                     |
| `04_PerspectiveProjection`          | Demonstrates perspective projection                                      |
| `05_Square`                         | Renders a simple 2D square                                               |
| `06_Two2DShapes`                    | Draws multiple 2D shapes                                                 |
| `07_2DRotation`                     | Applies 2D rotation transformation                                       |
| `08_3DRotation`                     | Applies 3D rotation                                                      |
| `09_Texture`                        | Texture mapping example                                                  |
| `10_Lights`                         | Basic lighting in OpenGL                                                 |
| `11_ShapesOnGraphPaper`             | Grid-aligned shapes rendering                                            |
| `12_PushPop`                        | OpenGL transformation stack usage                                        |
| `13_Tessellation`                   | Demonstrates tessellation shaders                                        |
| `14_GeometryShader`                 | Uses geometry shaders in pipeline                                        |
| `15_3DTexrureLightColor`            | 3D texture rendering with lighting & color                               |
| `15_Interleaved`                    | Uses interleaved vertex attributes                                       |
| `16_Indices`                        | Indexed drawing with element buffers                                     |
| `17_RenderToTexture_FBO`            | Framebuffer Object (FBO) for offscreen rendering                         |
| `18_ModelLoading`                   | Loads 3D models using external libraries                                 |


## ⚙️ CUDA & OpenCL Projects

- `CUDA/` — Parallel simulations (fluid, particles)  
- `OpenCL/` — GPGPU compute kernels  
- `CUDA_OpenGL_Interop/` — GPU interop with OpenGL for high-performance rendering
  
| 📂 Folder Name              | 📝 Description                                                              |
|----------------------------|------------------------------------------------------------------------------|
| `DevProp`                  | Queries and displays device properties using CUDA/OpenCL                    |
| `HelloCUDA`                | Basic CUDA "Hello World" to verify GPU programming environment              |
| `VecAdd`                   | CUDA implementation of vector addition                                      |
| `Sinwave`                  | Generates a sine wave using CUDA parallel computation                       |
| `MatMul`                   | Matrix multiplication using OpenCL                                          |
| `CUDA_OpenGL_Interop`      | Demonstrates CUDA and OpenGL interoperability for real-time visualization   |



## 📚 Data Structures & Sorting Algorithms

- `DataStructures/` —

| 📂 Folder Name                    | 📝 Description                                                              |
|----------------------------------|------------------------------------------------------------------------------|
| `01_DYANAMIC_MEMORY_TECHNIQUES` | Manual dynamic memory allocation and management in C/C++                   |
| `02_SINGLY_LINKED_LIST`         | Implementation of singly linked list with basic operations                 |
| `05_DOUBLE_CIRCULAR_LINKED_LIST`| Circular doubly linked list implementation                                 |
| `06_ISERTION_SORT`              | Insertion sort algorithm                                                   |
| `06_MERGE_SORT`                 | Merge sort implementation                                                  |
| `07_QUICK_SORT`                 | Quick sort implementation                                                  |
| `08_SELECTION_SORT`             | Selection sort example                                                     |
| `09_BUBBLE_SORT`                | Bubble sort algorithm                                                      |
| `11_STACK_USING_LINKED_LIST`   | Stack implemented using a linked list                                      |
| `15_GRAPH`                      | Graph representation and traversal methods (e.g., BFS/DFS)                 |
| `18_HASH_QUEUE`                 | Custom implementation of a hash queue                                      |
| `19_BST`                        | Binary Search Tree with insertion, deletion, and traversal operations      |


---

### 📦 Dependencies

- glew
- OpenGL / DirectX SDKs
- CUDA Toolkit (for CUDA projects)
- Android NDK (for Android)
- Xcode / iOS SDK (for iOS)

