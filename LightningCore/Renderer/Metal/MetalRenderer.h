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
    class DepthStencilState;
    class DepthStencilDescriptor;
}

namespace CA
{
    class MetalLayer;
    class MetalDrawable;
}

class MetalVertexBuffer;
class MetalTexture;
class SubTexture;
class MetalTexture;

#include "MetalShader.h"
#include <simd/simd.h>
#include "Primitives/MeshBuilder.h"
#include "Camera/Camera.h"
#include "Math/AAPLMathUtilities.h"
#include <vector>

class MetalRenderer
{
public:
    MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer);
    ~MetalRenderer();
    
    void BeginFrame();

    // Create quads with texture
    void CreateQuad(const char* p_FilePath, const simd::float3 &position);
    void CreateQuad(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position);
    void CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath);
    // Create Cube
    void CreateCube(const char* p_FilePath);
    
    void Render();
    
    void Commit();
    
    inline MTL::Device* GetMetalDevice() { return m_MetalDevice; }
    
    inline MTL::CommandBuffer* GetMetalCommandBuffer() { return m_MetalCommandBuffer; }
    
    inline void SetRenderPassDescriptor(MTL::RenderPassDescriptor* p_RenderPassDescriptor) { m_RenderPassDescriptor = p_RenderPassDescriptor; }
    
    inline MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder; }
    
    inline MTL::RenderCommandEncoder* GetMetalRenderCommandEncoder() { return m_RenderCommandEncoder; }
    
    inline void SetCamera(Camera camera) { m_Camera = camera; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::CommandQueue* m_MetalCommandQueue = nullptr;
    MTL::CommandBuffer* m_MetalCommandBuffer = nullptr;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
    MTL::DepthStencilState* m_DepthStencilState = nullptr;
    MTL::DepthStencilDescriptor* m_DepthStencilDescriptor;
    
    MetalVertexBuffer* m_TransformationBuffer = nullptr;
    
    MetalShader m_Shader;
    
    CA::MetalLayer* m_MetalLayer = nullptr;
    
    Mesh m_Mesh;
    
    std::vector<Mesh> m_Meshes;
    
    MeshBuilder m_MeshBuilder;
    Camera m_Camera;
};
#endif //METALRENDERER_H
