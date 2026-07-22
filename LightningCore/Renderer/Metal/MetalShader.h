//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include "Metal/MTLPixelFormat.hpp"
#include <string>

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
    
    explicit MetalShader(const std::string &filePath, MTL::Device* metalDevice, MTL::PixelFormat depthAttachmentPixelFormat);
    explicit MetalShader(const std::string &filePath, const char* vertexFunction, const char* fragmentFunction, MTL::Device* metalDevice, MTL::VertexDescriptor* vertexDescriptor, MTL::PixelFormat depthAttachmentPixelFormat);
    
    ~MetalShader();
    
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
    
    std::string m_FilePath;
    const std::string LoadShaderFile(const std::string& path) const;
};

#endif //METALSHADER_H
