//
// Created by Kian Marvi on 11/24/24.
//

#include "MetalRenderer.h"
#include "ShaderUniforms.h"
#include "QuartzCore/CAMetalLayer.hpp"
#include "Primitives/MeshBuilder.h"
#include "MetalTexture.h"
#include "Primitives/MeshBuilder.h"
#include "MetalVertexDescriptor.h"
#include "Primitives/Sprite.h"
#include "MetalShader.h"
#include "Scene/Scene.h"
#include "Scene/Component.h"

#include <GLFW/glfw3.h>
#include <Metal/Metal.hpp>

#include <print>
#include <cstddef>

MetalRenderer::MetalRenderer(MTL::Device* p_MetalDevice, CA::MetalLayer* p_MetalLayer)
: m_MetalDevice(p_MetalDevice),
  m_MetalLayer(p_MetalLayer),
  m_MetalCommandQueue(m_MetalDevice->newMTL4CommandQueue()),
  m_MetalCommandBuffer(m_MetalDevice->newCommandBuffer()),
  m_ArgumentTableDescriptor(MTL4::ArgumentTableDescriptor::alloc()->init()),
  m_ResidencySetDescriptor(MTL::ResidencySetDescriptor::alloc()->init()),
  m_DepthStencilDescriptor(MTL::DepthStencilDescriptor::alloc()->init()),
  m_FrameAvailableSharedEvent(m_MetalDevice->newSharedEvent()),
  b_EnableWireframe(false),
  m_LightComponent(LightComponent()),
  m_FrameNum(0),
  m_FrameIndex(0),
  m_UniformBufferIndex(0)
{
    assert(m_MetalDevice);
    m_UniformBuffers.reserve(s_MaxEntities);
    m_LightUniformBufferPool.reserve(s_MaxEntities);
    m_Lights.reserve(100);
    
    m_ResidencySet = m_MetalDevice->newResidencySet(m_ResidencySetDescriptor, nullptr);
    m_FrameAvailableSharedEvent->setSignaledValue(0);
    m_ArgumentTableDescriptor->setMaxBufferBindCount(10);
    m_ArgumentTableDescriptor->setMaxTextureBindCount(10);
    m_VertexArgumentTable = m_MetalDevice->newArgumentTable(m_ArgumentTableDescriptor, nullptr);
    m_FragmentArgumentTable = m_MetalDevice->newArgumentTable(m_ArgumentTableDescriptor, nullptr);
    
    if (m_ArgumentTableDescriptor)
    {
        m_ArgumentTableDescriptor->release();
        m_ArgumentTableDescriptor = nullptr;
    }
    
    if (m_ResidencySetDescriptor)
    {
        m_ResidencySetDescriptor->release();
        m_ResidencySetDescriptor = nullptr;
    }
    
    for (auto& metalCommandAllocators : m_MetalCommandAllocators)
    {
        metalCommandAllocators = m_MetalDevice->newCommandAllocator();
    }
    m_ResidencySet->requestResidency();
    m_MetalCommandQueue->addResidencySet(m_ResidencySet);
    
    for (int i = 0; i < s_MaxEntities; ++i)
    {
        m_UniformBuffers.emplace_back(m_MetalDevice->newBuffer(sizeof(Uniforms), MTL::ResourceStorageModeShared));
        m_LightUniformBufferPool.emplace_back(m_MetalDevice->newBuffer(sizeof(LightUniforms), MTL::ResourceStorageModeShared));
        m_ResidencySet->addAllocation(m_UniformBuffers.at(i));
        m_ResidencySet->addAllocation(m_LightUniformBufferPool.at(i));
    }
    
    for (uint8_t i = 0; i < s_MaxFramesInFlight; ++i)
    {
        m_LightPositions[i] = m_MetalDevice->newBuffer(sizeof(float3) * s_MaxLights , MTL::ResourceStorageModeShared);
    }
    
    m_DepthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLess);
    m_DepthStencilDescriptor->setDepthWriteEnabled(true);
    m_DepthStencilState = m_MetalDevice->newDepthStencilState(m_DepthStencilDescriptor);

    if (m_DepthStencilDescriptor)
    {
        m_DepthStencilDescriptor->release();
        m_DepthStencilDescriptor = nullptr;
    }
    
    MetalVertexDescriptor vertexDescriptorBuilder;
    
    m_3DVertexDescriptor = vertexDescriptorBuilder
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Pos))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Color))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Normals))
        .AddAttribute(MTL::VertexFormatFloat2, offsetof(Vertex3D, m_TexCoord))
        .SetBufferLayout(sizeof(Vertex3D))
        .BuildVertexDescriptor();
    
    
    m_LightVertexDescriptor = vertexDescriptorBuilder
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Pos))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Color))
        .AddAttribute(MTL::VertexFormatFloat3, offsetof(Vertex3D, m_Normals))
        .SetBufferLayout(sizeof(Vertex3D))
        .BuildVertexDescriptor();
    
    m_TextureShader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_UntexturedShader = new MetalShader("Assets/Shaders/Shader.metal", "vertex_main", "fragment_main_untextured", m_MetalDevice, m_3DVertexDescriptor, m_MetalLayer->pixelFormat());
    
    m_LightShader = new MetalShader("Assets/Shaders/Light.metal", "vertex_light", "fragment_light", m_MetalDevice, m_LightVertexDescriptor, m_MetalLayer->pixelFormat());
    
    if (m_3DVertexDescriptor)
    {
        m_3DVertexDescriptor->release();
        m_3DVertexDescriptor = nullptr;
    }
    if (m_LightVertexDescriptor)
    {
        m_LightVertexDescriptor->release();
        m_LightVertexDescriptor = nullptr;
    }
}

MetalRenderer::~MetalRenderer()
{
    for (uint8_t i = 0; i < s_MaxFramesInFlight; ++i)
    {
        m_LightPositions[i]->release();
        m_LightPositions[i] = nullptr;
    }
    
    for (int i = 0; i < s_MaxEntities; ++i)
    {
        if (m_UniformBuffers.at(i))
        {
            m_UniformBuffers.at(i)->release();
            m_UniformBuffers.at(i) = nullptr;
        }
        if (m_LightUniformBufferPool.at(i))
        {
            m_LightUniformBufferPool.at(i)->release();
            m_LightUniformBufferPool.at(i) = nullptr;
        }
    }
    
    if (m_DepthStencilState)
    {
        m_DepthStencilState->release();
        m_DepthStencilState = nullptr;
    }

    if (m_VertexArgumentTable)
    {
        m_VertexArgumentTable->release();
        m_VertexArgumentTable = nullptr;
    }

    if (m_FragmentArgumentTable)
    {
        m_FragmentArgumentTable->release();
        m_FragmentArgumentTable = nullptr;
    }

    if (m_MetalCommandBuffer)
    {
        m_MetalCommandBuffer->release();
        m_MetalCommandBuffer = nullptr;
    }
    
    for (auto &metalCommandAllocators : m_MetalCommandAllocators)
    {
        metalCommandAllocators->release();
        metalCommandAllocators = nullptr;
    }
    
    if (m_ResidencySet)
    {
        m_ResidencySet->release();
        m_ResidencySet = nullptr;
    }
    
    if (m_LightShader)
    {
        delete m_LightShader;
        m_LightShader = nullptr;
    }
    
    if (m_TextureShader)
    {
        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
    
    if (m_UntexturedShader)
    {
        delete m_UntexturedShader;
        m_UntexturedShader = nullptr;
    }
    
    if (m_MetalCommandQueue)
    {
        m_MetalCommandQueue->release();
        m_MetalCommandQueue = nullptr;
    }
    
    if (m_MetalLayer)
    {
        m_MetalLayer->release();
        m_MetalLayer = nullptr;
    }
    
    if (m_MetalDevice)
    {
        m_MetalDevice->release();
        m_MetalDevice = nullptr;
    }
    
}


void MetalRenderer::AddToResidencySet(const MTL::Allocation* p_Allocation)
{
    m_ResidencySet->addAllocation(p_Allocation);
}


void MetalRenderer::CommitResidencySet()
{
    m_ResidencySet->commit();
}

MeshHandle MetalRenderer::Create3DMesh(const Mesh_3D &mesh, const MetalTexture* texture)
{
    MTLMeshAttributes meshAttributes;
    
    meshAttributes.m_IndexCount = mesh.m_IndexCount;
    meshAttributes.m_VertexBuffer = m_MetalDevice->newBuffer(mesh.m_Vertices.data(), static_cast<uint32_t>(mesh.m_VertexSize), MTL::ResourceStorageModeShared);
    memcpy(meshAttributes.m_VertexBuffer->contents(), mesh.m_Vertices.data(), mesh.m_VertexSize);
    
    meshAttributes.m_IndexBuffer  = m_MetalDevice->newBuffer(mesh.m_Indices.data(), static_cast<uint32_t>(mesh.m_IndexSize), MTL::ResourceStorageModeShared);
    memcpy(meshAttributes.m_IndexBuffer->contents(), mesh.m_Indices.data(), mesh.m_IndexSize);
    
    // Add the vertex and index buffer to residency set
    m_ResidencySet->addAllocation(meshAttributes.m_VertexBuffer);
    m_ResidencySet->addAllocation(meshAttributes.m_IndexBuffer);
    
    if (texture)
    {
        m_ResidencySet->addAllocation(texture->GetArgumentBuffer());
        m_ResidencySet->addAllocation(texture->GetTexture());
    }
    
    m_RenderMeshes.emplace_back(meshAttributes);
    
    return m_RenderMeshes.size() - 1;
}


void MetalRenderer::Submit(const Camera &camera, const float aspectRatio)
{
    m_Camera = camera;
    m_UniformBufferIndex = 0;
    
    if (m_FrameNum >= s_MaxFramesInFlight)
    {
        m_FrameAvailableSharedEvent->waitUntilSignaledValue(m_FrameNum - s_MaxFramesInFlight, 33);
    }
    
    m_FrameIndex = m_FrameNum % s_MaxFramesInFlight;
    m_MetalCommandAllocators[m_FrameIndex]->reset();
    m_MetalCommandBuffer->beginCommandBuffer(m_MetalCommandAllocators[m_FrameIndex]);
    
    m_RenderPassDescriptor->depthAttachment()->setClearDepth(1.0f);
    
    m_RenderCommandEncoder = m_MetalCommandBuffer->renderCommandEncoder(m_RenderPassDescriptor);
    m_RenderCommandEncoder->setDepthStencilState(m_DepthStencilState);
    m_RenderCommandEncoder->setFrontFacingWinding(MTL::WindingClockwise);
    m_RenderCommandEncoder->setCullMode(MTL::CullModeBack);
    
    if (b_EnableWireframe)
    {
        m_RenderCommandEncoder->setTriangleFillMode(MTL::TriangleFillModeLines);
    }
    else
    {
        m_RenderCommandEncoder->setTriangleFillMode(MTL::TriangleFillModeFill);
    }
    
    m_ViewMatrix = m_Camera.GetViewMatrix();
    
    float fov = m_Camera.GetZoom() * (M_PI / 180.0f);
    
    m_ProjectionMatrix = float4x4::perspective(projection(frustum::field_of_view_x(fov, aspectRatio, 0.1f, 1000.f), zclip::zero, zdirection::forward, zplane::finite));
}

void MetalRenderer::RenderLights(const float4x4 &modelMatrix, const MeshHandle meshHandle, const LightComponent &p_LightComponent)
{
    if (!m_LightShader)
    {
        std::println("Light shader is not initialised");
        return;
    }
    
    if (meshHandle >= m_RenderMeshes.size())
        return;
    
    
    m_ModelMatrix = modelMatrix;
    m_LightComponent = p_LightComponent;
    m_Uniforms  = {m_ProjectionMatrix, m_ViewMatrix, m_ModelMatrix};
    memcpy(m_UniformBuffers[m_UniformBufferIndex]->contents(), &m_Uniforms, sizeof(m_Uniforms));
    
    float3 color = m_LightComponent.m_Color;
    memcpy(m_LightPositions[m_UniformBufferIndex]->contents(), &color, sizeof(color));
    
    m_LightComponent.m_Position = float3(modelMatrix[3].x,
                                         modelMatrix[3].y,
                                         modelMatrix[3].z);
    

    m_RenderCommandEncoder->setRenderPipelineState(m_LightShader->GetRenderPipelineState());
    m_VertexArgumentTable->setAddress(m_RenderMeshes[meshHandle].m_VertexBuffer->gpuAddress(), 0);
    m_VertexArgumentTable->setAddress(m_UniformBuffers[m_UniformBufferIndex]->gpuAddress(), 1);
    m_FragmentArgumentTable->setAddress(m_LightPositions[m_UniformBufferIndex]->gpuAddress(), 0);
    m_RenderCommandEncoder->setArgumentTable(m_VertexArgumentTable, MTL::RenderStageVertex);
    m_RenderCommandEncoder->setArgumentTable(m_FragmentArgumentTable, MTL::RenderStageFragment);
    m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                  m_RenderMeshes[meshHandle].m_IndexCount,
                                                  MTL::IndexTypeUInt16,
                                                  m_RenderMeshes[meshHandle].m_IndexBuffer->gpuAddress(),
                                                  m_RenderMeshes[meshHandle].m_IndexBuffer->length());
    ++m_UniformBufferIndex;
}

void MetalRenderer::RenderMesh(const float4x4& modelMatrix, const MeshHandle meshHandle, const MetalTexture* texture)
{
    if (meshHandle >= m_RenderMeshes.size())
        return;

    m_ModelMatrix = modelMatrix;
    m_Uniforms  = {m_ProjectionMatrix, m_ViewMatrix, m_ModelMatrix};
    memcpy(m_UniformBuffers.at(m_UniformBufferIndex)->contents(), &m_Uniforms, sizeof(m_Uniforms));

    m_LightUniforms = { m_LightComponent.m_Color , m_LightComponent.m_Position, m_Camera.GetPosition()};
    memcpy(m_LightUniformBufferPool.at(m_UniformBufferIndex)->contents(), &m_LightUniforms, sizeof(m_LightUniforms));
    
    m_RenderCommandEncoder->setRenderPipelineState(texture ? m_TextureShader->GetRenderPipelineState() : m_UntexturedShader->GetRenderPipelineState());
    
    m_VertexArgumentTable->setAddress(m_RenderMeshes[meshHandle].m_VertexBuffer->gpuAddress(), 0);
    m_VertexArgumentTable->setAddress(m_UniformBuffers.at(m_UniformBufferIndex)->gpuAddress(), 1);

    if (texture)
    {
        m_FragmentArgumentTable->setAddress(texture->GetArgumentBuffer()->gpuAddress(), 0);
        m_FragmentArgumentTable->setAddress(m_LightUniformBufferPool[m_UniformBufferIndex]->gpuAddress(), 1);
    }
    else
    {
        m_FragmentArgumentTable->setAddress(m_LightUniformBufferPool[m_UniformBufferIndex]->gpuAddress(), 0);
    }

    m_RenderCommandEncoder->setArgumentTable(m_VertexArgumentTable, MTL::RenderStageVertex);
    m_RenderCommandEncoder->setArgumentTable(m_FragmentArgumentTable, MTL::RenderStageFragment);
    m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle,
                                                  m_RenderMeshes[meshHandle].m_IndexCount,
                                                  MTL::IndexTypeUInt16,
                                                  m_RenderMeshes[meshHandle].m_IndexBuffer->gpuAddress(),
                                                  m_RenderMeshes[meshHandle].m_IndexBuffer->length());
    
    ++m_UniformBufferIndex;
}

void MetalRenderer::Commit()
{
    m_RenderCommandEncoder->endEncoding();
    m_MetalCommandBuffer->endCommandBuffer();
    m_MetalCommandQueue->wait(m_Drawable);
    m_MetalCommandQueue->commit(&m_MetalCommandBuffer, 1);
    m_MetalCommandQueue->signalDrawable(m_Drawable);
    m_MetalCommandQueue->signalEvent(m_FrameAvailableSharedEvent, m_FrameNum);
    ++m_FrameNum;
}
