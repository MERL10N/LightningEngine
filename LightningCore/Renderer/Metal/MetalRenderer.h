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

#include "Camera/Camera.h"
#include "Scene/Component.h"
#include "ShaderUniforms.h"
#include <vector>

static constexpr uint16_t MAX_ENTITIES         = 10000;
static constexpr uint8_t  MAX_FRAMES_IN_FLIGHT = 3;
static constexpr unsigned int MAX_INSTANCES    = 1000;


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

    // Create quads with texture
    void CreateQuad(const char* p_FilePath, const float3 &position);
    void CreateQuad(const char* p_FilePath, const float3 &scale, const float3 &position);
    void CreateQuad(const float2 &position, const float2 &size, const char* p_FilePath);
    
    MeshHandle Create3DMesh(const Mesh_3D &mesh, const MetalTexture* texture);
    
    // Scene rendering
    void AddToResidencySet(const MTL::Allocation* p_Allocation);
    void RegisterMesh(const Mesh_3D &p_3DMesh);
    void RegisterTexture(const MetalTexture *p_Texture);
    void CommitResidencySet();
    
    void Submit(const Camera &p_Camera, const float p_AspectRatio);
    void RenderLights(const float4x4 &p_ModelMatrix, const MeshHandle p_MeshHandle, const LightComponent &p_LightComponent);
    void RenderMesh(const float4x4 &p_ModelMatrix, const MeshHandle p_MeshHandle, const MetalTexture *p_Texture);
    void Commit();
    
    inline const MTL::Device*                GetMetalDevice()               const { return m_MetalDevice; }
    inline const MTL::ResidencySet*          GetMetalResidencySet()         const { return m_ResidencySet; }
    inline const MTL4::RenderPassDescriptor* GetMetalRenderPassDescriptor() const { return m_RenderPassDescriptor; }
    inline const MTL4::RenderCommandEncoder* GetMetalRenderCommandEncoder() const { return m_RenderCommandEncoder; }
    inline       MTL4::CommandBuffer*        GetMetalCommandBuffer()        const { return m_MetalCommandBuffer; }
    inline       MTL4::CommandQueue*         GetMetalCommandQueue()         const { return m_MetalCommandQueue; }

    
    inline void SetRenderPassDescriptor(MTL4::RenderPassDescriptor* p_RenderPassDescriptor) { m_RenderPassDescriptor = p_RenderPassDescriptor; }
    inline void SetRenderCommandEncoder(MTL4::RenderCommandEncoder* p_RenderCommandEncoder) { m_RenderCommandEncoder = p_RenderCommandEncoder; }
    inline void SetWireframeMode(const bool p_EnableWireFrame)                              { b_EnableWireframe = p_EnableWireFrame; }
    inline void SetMetalDrawable(MTL::Drawable* p_Drawable)                                 { m_Drawable = p_Drawable; }

private:

    MTL::Device*                    m_MetalDevice               = nullptr;
    CA::MetalLayer*                 m_MetalLayer                = nullptr;
    
    MTL4::CommandQueue*             m_MetalCommandQueue         = nullptr;
    MTL4::CommandBuffer*            m_MetalCommandBuffer        = nullptr;
    
    MTL4::CommandAllocator*         m_MetalCommandAllocators[MAX_FRAMES_IN_FLIGHT];
    
    MTL4::RenderPassDescriptor*     m_RenderPassDescriptor      = nullptr;
    MTL4::RenderCommandEncoder*     m_RenderCommandEncoder      = nullptr;
    MTL4::ArgumentTable*            m_VertexArgumentTable       = nullptr;
    MTL4::ArgumentTable*            m_FragmentArgumentTable     = nullptr;
    MTL4::ArgumentTableDescriptor*  m_ArgumentTableDescriptor   = nullptr;
    
    MTL::ResidencySet*              m_ResidencySet              = nullptr;
    MTL::ResidencySetDescriptor*    m_ResidencySetDescriptor    = nullptr;
    MTL::DepthStencilState*         m_DepthStencilState         = nullptr;
    MTL::DepthStencilDescriptor*    m_DepthStencilDescriptor    = nullptr;
    MTL::VertexDescriptor*          m_3DVertexDescriptor        = nullptr;
    MTL::VertexDescriptor*          m_LightVertexDescriptor     = nullptr;
    MTL::Drawable*                  m_Drawable                  = nullptr;
    
    MTL::SharedEvent*               m_FrameAvailableSharedEvent = nullptr;
    
    MTL::Buffer*                    m_UniformBuffer             = nullptr;
    MTL::Buffer*                    m_LightUniformBuffer        = nullptr;
    MTL::Buffer*                    m_InstanceBuffer            = nullptr;
    
    std::vector<MTL::Buffer*>       m_UniformBuffers;
    std::vector<MTL::Buffer*>       m_LightUniformBufferPool;
    std::vector<MTLMeshAttributes>  m_RenderMeshes;
    
    MetalShader*                    m_TextureShader             = nullptr;
    MetalShader*                    m_UntexturedShader          = nullptr;
    MetalShader*                    m_LightShader               = nullptr;
    
    
    size_t m_UniformBufferIndex;
    size_t m_FrameNum;
    size_t m_FrameIndex;
    
    Camera m_Camera;
    
    bool b_EnableWireframe = false;
    
    float4x4 m_ViewMatrix;
    float4x4 m_ProjectionMatrix;
    float4x4 m_ModelMatrix;
    float4x4 m_LightPosition;
    
    LightComponent  m_LightComponent;
    
    LightUniforms   m_LightUniforms;
    Uniforms        m_Uniforms;
};
#endif //METALRENDERER_H
