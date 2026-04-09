//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include "Metal/MTLPixelFormat.hpp"
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
    class RenderCommandEncoder;
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
    
    MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    MetalShader(const std::string &p_FilePath, const char* p_VertexFunction, const char* p_FragmentFunction, MTL::Device* p_MetalDevice, MTL::VertexDescriptor* p_VertexDescriptor, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    
    ~MetalShader();
    
    
    void SetDepthAttachmentPixelFormat(MTL::PixelFormat p_PixelFormat);
    
    void SetFragmentShaderUniformMatrix4x4(MTL::RenderCommandEncoder* encoder, const matrix_float4x4& value, const int index);

    void SetVertexShaderUniformMatrix4x4(MTL::RenderCommandEncoder* encoder, const matrix_float4x4& value, const int index);

    
    inline MTL::RenderPipelineState* GetRenderPipelineState(){ return m_RenderPipelineState; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Library* m_Library = nullptr;
    MTL::Function* m_VertexFunction = nullptr;
    MTL::Function* m_FragmentFunction = nullptr;
    MTL::RenderPipelineDescriptor* m_RenderPipelineDescriptor = nullptr;
    MTL::RenderPipelineState* m_RenderPipelineState = nullptr;
    MTL::DepthStencilState* m_DepthStencilState = nullptr;
    MTL::DepthStencilDescriptor* m_DepthStencilDescriptor = nullptr;
    MTL::VertexDescriptor* m_VertexDescriptor = nullptr;
    MTL::RenderPipelineColorAttachmentDescriptor* m_ColorAttachmentDescriptor = nullptr;
    MTL::Buffer* m_ArgumentBuffer = nullptr;
    MTL::ArgumentEncoder* m_ArgumentEncoder = nullptr;
    MTL::PixelFormat m_DepthAttachmentPixelFormat;
    std::string m_FilePath;
    std::string LoadShaderFile(const std::string& path);
    
};



#endif //METALSHADER_H
