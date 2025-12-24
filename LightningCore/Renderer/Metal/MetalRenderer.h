//
// Created by Kian Marvi on 11/24/24.
//

#ifndef METALRENDERER_H
#define METALRENDERER_H

namespace MTL
{
    class Device;
    class CommandQueue;
    class CommandBuffer;
    class RenderPassDescriptor;
    class RenderCommandEncoder;
    class Buffer;
    class RenderPassColorAttachmentDescriptor;
}

namespace CA
{
    class MetalLayer;
    class MetalDrawable;
}

#include "MetalShader.h"
#include <simd/simd.h>
#include "Primitives/MeshBuilder.h"

class MetalVertexBuffer;
class MetalTexture;

class MetalRenderer
{
public:
    MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer);
    ~MetalRenderer();
    
    void BeginFrame();

    void CreateQuad(const char* p_FilePath);
    void CreateCube(const char* p_FilePath);
    
    void Render();
    
    void Commit();
    
    inline MTL::Device* GetMetalDevice() { return m_MetalDevice; }
    
    inline MTL::CommandBuffer* GetMetalCommandBuffer() { return m_MetalCommandBuffer; }
    
    inline void SetRenderPassDescriptor(MTL::RenderPassDescriptor* p_RenderPassDescriptor) { m_RenderPassDescriptor = p_RenderPassDescriptor; }
    
    inline MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder; }
    
    inline MTL::RenderCommandEncoder* GetMetalRenderCommandEncoder() { return m_RenderCommandEncoder; }

private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::CommandQueue* m_MetalCommandQueue = nullptr;
    MTL::CommandBuffer* m_MetalCommandBuffer = nullptr;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
    
    MetalVertexBuffer* m_TransformationBuffer = nullptr;
    
    MetalShader m_Shader;
    
    CA::MetalLayer* m_MetalLayer = nullptr;
    
    Mesh m_QuadMesh, m_CubeMesh;
    MeshBuilder m_MeshBuilder;
};
#endif //METALRENDERER_H
