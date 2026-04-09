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
    class VertexDescriptor;
    class TextureDescriptor;
    class Texture;
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
class MetalShader;
class Scene;

struct Mesh_3D;
struct Mesh_2D;

#include <simd/simd.h>
#include "Primitives/Sprite.h"
#include "Camera/Camera.h"
#include "Math/AAPLMathUtilities.h"
#include <vector>

class MetalRenderer
{
public:
    MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer);
    ~MetalRenderer();
    
    
    inline MTL::Device* GetMetalDevice() { return m_MetalDevice; }
    
    inline MTL::CommandBuffer* GetMetalCommandBuffer() { return m_MetalCommandBuffer; }
    
    inline void SetRenderPassDescriptor(MTL::RenderPassDescriptor* p_RenderPassDescriptor) { m_RenderPassDescriptor = p_RenderPassDescriptor; }
    
    inline MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder; }
    
    inline void SetWireframeMode(bool p_EnableWireFrame) { b_EnableWireframe = p_EnableWireFrame; }
    
    inline MTL::RenderCommandEncoder* GetMetalRenderCommandEncoder() { return m_RenderCommandEncoder; }
    
    void SubmitCommandBuffer();
    

    // Create quads with texture
    void CreateQuad(const char* p_FilePath, const simd::float3 &position);
    void CreateQuad(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position);
    void CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath);
    void CreateSprite(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position, const Sprite &sprite);
    
    // Scene rendering
    void BeginScene(const Camera &p_Camera, const float p_AspectRatio);
    void Render(const matrix_float4x4& p_Transform, const Mesh_3D& p_3DMesh, const MetalTexture* p_Texture);
    void EndScene();

    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::CommandQueue* m_MetalCommandQueue = nullptr;
    MTL::CommandBuffer* m_MetalCommandBuffer = nullptr;
    MTL::RenderPassDescriptor* m_RenderPassDescriptor = nullptr;
    MTL::RenderCommandEncoder* m_RenderCommandEncoder = nullptr;
    MTL::DepthStencilState* m_DepthStencilState = nullptr;
    MTL::DepthStencilDescriptor* m_DepthStencilDescriptor = nullptr;
    MTL::VertexDescriptor* m_3DVertexDescriptor = nullptr;
    MTL::VertexDescriptor* m_LightVertexDescriptor = nullptr;
    
    
    MTL::Buffer* m_ArgumentBuffer = nullptr;
    
    MetalShader* m_Shader;
    MetalShader* m_LightShader;
    
    CA::MetalLayer* m_MetalLayer = nullptr;

    std::vector<Mesh_2D> m_2DMeshes;
    std::vector<Mesh_3D> m_3DMeshes;

    Camera m_Camera;
    
    bool b_EnableWireframe;
};
#endif //METALRENDERER_H
