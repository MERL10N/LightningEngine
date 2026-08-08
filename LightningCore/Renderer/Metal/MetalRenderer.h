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
    class Buffer;
    class RenderPassColorAttachmentDescriptor;
    class DepthStencilState;
    class DepthStencilDescriptor;
    class VertexDescriptor;
    class TextureDescriptor;
    class Texture;
    class ResidencySet;
    class ResidencySetDescriptor;
    class Drawable;
    class SharedEvent;
    class Allocation;
}

namespace MTL4
{
    class ArgumentTable;
    class ArgumentTableDescriptor;
    class CommandBuffer;
    class CommandQueue;
    class CommandAllocator;
    class RenderPassDescriptor;
    class RenderCommandEncoder;
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
class Sprite;

struct Mesh_3D;
struct Mesh_2D;

#include "Scene/Component.h"
#include "ShaderUniforms.h"
#include "MetalShader.h"
#include <vector>

static constexpr uint8_t  s_MaxFramesInFlight   = 3;
static constexpr uint8_t  s_MaxLights           = 10;
static constexpr uint16_t s_MaxInstances        = 1000;
static constexpr uint16_t s_MaxEntities         = 10000;


struct MTLMeshAttributes
{
    MTL::Buffer*   m_VertexBuffer  = nullptr;
    MTL::Buffer*   m_IndexBuffer   = nullptr;
    uint16_t       m_IndexCount    = 0;
};

class MetalRenderer
{
public:
    MetalRenderer() = default;
    explicit MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer);
    ~MetalRenderer();

    // Create 3D Mesh with or without a texture
    MeshHandle Create3DMesh(const Mesh_3D &mesh, const MetalTexture *texture);
    
    // Scene rendering
    void AddToResidencySet(const MTL::Allocation* p_Allocation);
    void CommitResidencySet();
    
    void Submit(const Camera &p_Camera, const float p_AspectRatio);
    void RenderLights(const float4x4 &p_ModelMatrix, const MeshHandle p_MeshHandle, const LightComponent &p_LightComponent);
    void RenderMesh(const float4x4 &p_ModelMatrix, const MeshHandle p_MeshHandle, MetalTexture *p_Texture);
    void Commit();
    
    // Getters and Setters
    inline const MTL::ResidencySet*          GetMetalResidencySet()         const { return m_ResidencySet; }
    inline const MTL4::RenderPassDescriptor* GetMetalRenderPassDescriptor() const { return m_RenderPassDescriptor; }
    inline const MTL4::RenderCommandEncoder* GetMetalRenderCommandEncoder() const { return m_RenderCommandEncoder; }
    inline       MTL4::CommandBuffer*        GetMetalCommandBuffer()        const { return m_MetalCommandBuffer; }
    inline       MTL4::CommandQueue*         GetMetalCommandQueue()         const { return m_MetalCommandQueue; }

    
    inline void SetRenderPassDescriptor(const MTL4::RenderPassDescriptor* renderPassDescriptor) { m_RenderPassDescriptor = renderPassDescriptor; }
    inline void SetRenderCommandEncoder(MTL4::RenderCommandEncoder* p_RenderCommandEncoder)     { m_RenderCommandEncoder = p_RenderCommandEncoder; }
    inline void SetWireframeMode(const bool enableWireFrame)                                    { b_EnableWireframe = enableWireFrame; }
    inline void SetMetalDrawable(const MTL::Drawable* drawable)                                 { m_Drawable = drawable; }

private:

    MTL::Device*                          m_MetalDevice               = nullptr;
    CA::MetalLayer*                       m_MetalLayer                = nullptr;
    MTL4::CommandQueue*                   m_MetalCommandQueue         = nullptr;
    MTL4::CommandBuffer*                  m_MetalCommandBuffer        = nullptr;
    MTL4::CommandAllocator*               m_MetalCommandAllocators[s_MaxFramesInFlight];
    const MTL4::RenderPassDescriptor*     m_RenderPassDescriptor      = nullptr;
    MTL4::RenderCommandEncoder*           m_RenderCommandEncoder      = nullptr;
    MTL4::ArgumentTable*                  m_VertexArgumentTable       = nullptr;
    MTL4::ArgumentTable*                  m_FragmentArgumentTable     = nullptr;
    MTL4::ArgumentTableDescriptor*        m_ArgumentTableDescriptor   = nullptr;

    MTL::ResidencySet*              m_ResidencySet              = nullptr;
    MTL::ResidencySetDescriptor*    m_ResidencySetDescriptor    = nullptr;
    MTL::DepthStencilState*         m_DepthStencilState         = nullptr;
    MTL::DepthStencilDescriptor*    m_DepthStencilDescriptor    = nullptr;
    MTL::VertexDescriptor*          m_3DVertexDescriptor        = nullptr;
    MTL::VertexDescriptor*          m_LightVertexDescriptor     = nullptr;
    const MTL::Drawable*            m_Drawable                  = nullptr;
    
    MTL::SharedEvent*               m_FrameAvailableSharedEvent = nullptr;
    
    MTL::Buffer*                    m_UniformBuffer             = nullptr;
    MTL::Buffer*                    m_LightUniformBuffer        = nullptr;
    MTL::Buffer*                    m_InstanceBuffer            = nullptr;
    MTL::Buffer*                    m_LightPositions[s_MaxFramesInFlight];
    
    std::vector<MTL::Buffer*>       m_UniformBuffers;
    std::vector<MTL::Buffer*>       m_LightUniformBufferPool;
    std::vector<MTLMeshAttributes>  m_RenderMeshes;
    std::vector<LightUniforms>      m_Lights;

    
    MetalShader                    m_TextureShader;
    MetalShader                    m_UntexturedShader;
    MetalShader                    m_LightShader;
    
    bool   b_EnableWireframe = false;
    size_t m_UniformBufferIndex;
    size_t m_FrameNum;
    size_t m_FrameIndex;
    
    float3   m_CameraPosition;
    
    float4x4 m_ViewMatrix;
    float4x4 m_ProjectionMatrix;
    float4x4 m_ModelMatrix;
    float4x4 m_LightPosition;
    
    LightComponent  m_LightComponent;
    
    LightUniforms   m_LightUniforms;
    Uniforms        m_Uniforms;
};
#endif //METALRENDERER_H
