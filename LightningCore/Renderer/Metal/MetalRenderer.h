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


#include <simd/simd.h>
#include "Camera/Camera.h"
#include "Math/AAPLMathUtilities.h"
#include "Scene/Component.h"
#include "ShaderUniforms.h"
#include <vector>

class MetalRenderer
{
public:
    MetalRenderer() = default;
    explicit MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer);
    ~MetalRenderer();

    // Create quads with texture
    void CreateQuad(const char* p_FilePath, const simd::float3 &position);
    void CreateQuad(const char* p_FilePath, const simd::float3 &scale, const simd::float3 &position);
    void CreateQuad(const simd::float2 &position, const simd::float2 &size, const char* p_FilePath);
    
    // Scene rendering
    void AddToResidencySet(const MTL::Allocation* p_Allocation);
    void RegisterMesh(const Mesh_3D &p_3DMesh);
    void RegisterTexture(const MetalTexture* p_Texture);
    void CommitResidencySet();
    
    void Submit(const Camera &p_Camera, const float p_AspectRatio);
    void RenderLights(const matrix_float4x4 &p_ModelMatrix, const Mesh_3D &p_3DMesh, const LightComponent &p_LightComponent);
    void RenderMesh(const matrix_float4x4 &p_ModelMatrix, const Mesh_3D &p_3DMesh, const MetalTexture* p_Texture);
    void Commit();
    
    inline const MTL::Device* GetMetalDevice() const { return m_MetalDevice; }

    inline MTL4::CommandBuffer* GetMetalCommandBuffer() const { return m_MetalCommandBuffer; }
    
    inline MTL4::CommandQueue* GetMetalCommandQueue() const { return m_MetalCommandQueue; }
    
    inline MTL::ResidencySet* GetMetalResidencySet() const { return m_ResidencySet; }
    
    inline void SetRenderPassDescriptor(MTL4::RenderPassDescriptor* p_RenderPassDescriptor) { m_RenderPassDescriptor = p_RenderPassDescriptor; }
    
    inline const MTL4::RenderPassDescriptor* GetMetalRenderPassDescriptor() const { return m_RenderPassDescriptor; }
    
    inline void SetRenderCommandEncoder(MTL4::RenderCommandEncoder* p_RenderCommandEncoder) {  m_RenderCommandEncoder = p_RenderCommandEncoder;}
    
    inline void SetWireframeMode(const bool p_EnableWireFrame) { b_EnableWireframe = p_EnableWireFrame; }
    
    inline MTL4::RenderCommandEncoder* GetMetalRenderCommandEncoder() const { return m_RenderCommandEncoder; }
    
    inline void SetMetalDrawable(MTL::Drawable* p_Drawable) { m_Drawable = p_Drawable; }

private:
    MTL::Device*                    m_MetalDevice               = nullptr;
    CA::MetalLayer*                 m_MetalLayer                = nullptr;
    
    MTL4::CommandQueue*             m_MetalCommandQueue         = nullptr;
    MTL4::CommandBuffer*            m_MetalCommandBuffer        = nullptr;
    MTL4::CommandAllocator*         m_MetalCommandAllocators[3];
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
    
    MetalShader*                    m_TextureShader             = nullptr;
    MetalShader*                    m_UntexturedShader          = nullptr;
    MetalShader*                    m_LightShader               = nullptr;

    Camera m_Camera;
    
    bool b_EnableWireframe = false;
    
    matrix_float4x4 m_ViewMatrix;
    matrix_float4x4 m_PerspectiveMatrix;
    matrix_float4x4 m_ModelMatrix;
    
    LightComponent  m_LightComponent;
    matrix_float4x4 m_LightPosition;
    
    LightUniforms   m_LightUniforms;
    Uniforms        m_Uniforms;
    
    size_t          m_FrameNum;
    size_t          m_FrameIndex;
};
#endif //METALRENDERER_H
