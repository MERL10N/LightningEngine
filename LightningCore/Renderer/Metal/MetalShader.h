//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include "Metal/MTLPixelFormat.hpp"
#include "Metal/MTLRenderCommandEncoder.hpp"
#include <string>
#include <simd/simd.h>
namespace MTL
{
    class Device;
    class RenderPipelineState;
    class RenderPipelineDescriptor;
    class Texture;
    class Function;
    class Library;
    class DepthStencilState;
    class DepthStencilDescriptor;
    class RenderPassDescriptor;
    class VertexDescriptor;
    class RenderPipelineColorAttachmentDescriptor;
    class Buffer;
    class ArgumentEncoder;
   
}

class MetalShader
{
public:
    MetalShader() = default;
    
    explicit MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    explicit MetalShader(const std::string &p_FilePath, const char* p_VertexFunction, const char* p_FragmentFunction, MTL::Device* p_MetalDevice, MTL::VertexDescriptor* p_VertexDescriptor, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    
    ~MetalShader();
    
    
    void SetDepthAttachmentPixelFormat(MTL::PixelFormat p_PixelFormat);
    
    template<typename T>
    inline void SetFragmentShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index)
    {
        encoder->setFragmentBytes(&value, sizeof(value), index);
    }

    template<typename T>
    inline void SetVertexShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index)
    {
        encoder->setVertexBytes(&value, sizeof(value), index);
    }


    MTL::Buffer* InitialiseArgumentBuffers(const MTL::Texture* p_Texture) const;
    
    inline const MTL::RenderPipelineState* GetRenderPipelineState() const { return m_RenderPipelineState; }
    
private:
    MTL::Device*                                    m_MetalDevice               = nullptr;
    MTL::Library*                                   m_Library                   = nullptr;
    MTL::Function*                                  m_VertexFunction            = nullptr;
    MTL::Function*                                  m_FragmentFunction          = nullptr;
    MTL::RenderPipelineDescriptor*                  m_RenderPipelineDescriptor  = nullptr;
    MTL::RenderPipelineState*                       m_RenderPipelineState       = nullptr;
    MTL::DepthStencilState*                         m_DepthStencilState         = nullptr;
    MTL::DepthStencilDescriptor*                    m_DepthStencilDescriptor    = nullptr;
    MTL::VertexDescriptor*                          m_VertexDescriptor          = nullptr;
    MTL::RenderPipelineColorAttachmentDescriptor*   m_ColorAttachmentDescriptor = nullptr;
    
    MTL::PixelFormat m_DepthAttachmentPixelFormat;
    
    std::string s_FilePath;
    const std::string LoadShaderFile(const std::string& path) const;
};

#endif //METALSHADER_H
