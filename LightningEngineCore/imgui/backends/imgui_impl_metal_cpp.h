//
//  imgui_impl_metal_cpp.hpp
//  Metal-Tutorial
//
//  Created by Kian Marvi on 31/1/24.
//

#ifndef imgui_impl_metal_cpp_hpp
#define imgui_impl_metal_cpp_hpp

#include "imgui.h"

#if defined(__APPLE__)

namespace MTL {
class Device;
class RenderPassDescriptor;
class CommandBuffer;
class RenderCommandEncoder;
}

IMGUI_IMPL_API bool
ImGui_ImplMetal_Init(MTL::Device* device);

IMGUI_IMPL_API void
ImGui_ImplMetal_Shutdown();

IMGUI_IMPL_API void
ImGui_ImplMetal_NewFrame(MTL::RenderPassDescriptor* renderPassDescriptor);

IMGUI_IMPL_API void
ImGui_ImplMetal_RenderDrawData(ImDrawData*                drawData,
                               MTL::CommandBuffer*        commandBuffer,
                               MTL::RenderCommandEncoder* commandEncoder);

#endif // #if defined(__APPLE__)

#endif /* imgui_impl_metal_cpp_hpp */
