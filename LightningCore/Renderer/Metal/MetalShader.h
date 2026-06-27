//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include "Metal/MTLPixelFormat.hpp"
#include "Metal/MTLRenderCommandEncoder.hpp"
#include "Metal/MTL4ArgumentTable.hpp"
#include <string>
#include <simd/simd.h>

namespace MTL
{
    class Device;
    class Texture;
    class Function;
    class Library;
    class DepthStencilState;
    class DepthStencilDescriptor;
    class RenderPassDescriptor;
    class VertexDescriptor;
    class RenderPipelineColorAttachmentDescriptor;
    class RenderPipelineState;
    class Buffer;
    class ArgumentEncoder;
}

namespace MTL4
{
    class LibraryFunctionDescriptor;
    class RenderPipelineDescriptor;
    class RenderPipelineColorAttachmentDescriptor;
    class Compiler;
    class CompilerDescriptor;
    class RenderCommandEncoder;
    class ArgumentTable;
}

class MetalShader
{
public:
    MetalShader() = default;
    
    explicit MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    explicit MetalShader(const std::string &p_FilePath, const char* p_VertexFunction, const char* p_FragmentFunction, MTL::Device* p_MetalDevice, MTL::VertexDescriptor* p_VertexDescriptor, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    
    ~MetalShader();
    
    
    void SetDepthAttachmentPixelFormat(MTL::PixelFormat p_PixelFormat);
    

    MTL::Buffer* InitialiseArgumentBuffers(const MTL::Texture* p_Texture) const;
    
    inline const MTL::RenderPipelineState* GetRenderPipelineState() const { return m_RenderPipelineState; }
    
private:
    MTL::Device*                                    m_MetalDevice                = nullptr;
    MTL::Library*                                   m_Library                    = nullptr;
    MTL4::CompilerDescriptor*                       m_CompilerDescriptor         = nullptr;
    MTL4::Compiler*                                 m_Compiler                   = nullptr;
    MTL4::LibraryFunctionDescriptor*                m_VertexFunction             = nullptr;
    MTL4::LibraryFunctionDescriptor*                m_FragmentFunction           = nullptr;
    MTL4::RenderPipelineDescriptor*                 m_RenderPipelineDescriptor   = nullptr;
    MTL::RenderPipelineState*                       m_RenderPipelineState        = nullptr;
    MTL::DepthStencilState*                         m_DepthStencilState          = nullptr;
    MTL::DepthStencilDescriptor*                    m_DepthStencilDescriptor     = nullptr;
    MTL::VertexDescriptor*                          m_VertexDescriptor           = nullptr;
    MTL4::RenderPipelineColorAttachmentDescriptor*  m_ColorAttachmentDescriptor  = nullptr;
    
    MTL::PixelFormat                                m_DepthAttachmentPixelFormat;
    
    std::string s_FilePath;
    const std::string LoadShaderFile(const std::string& path) const;
};

#endif //METALSHADER_H
