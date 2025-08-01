//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

#include <Metal/MTLPixelFormat.hpp>
#include <string>

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
   
}

class MetalShader
{
    public:
    MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat);
    MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, const char* p_VertexFunction, const char* p_FragmentFunction);
    ~MetalShader();
    
    void SetDepthAttachmentPixelFormat(MTL::PixelFormat p_PixelFormat);
    
    template <typename T>
    void SetFragmentShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    template <typename T>
    void SetVertexShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    inline MTL::RenderPipelineState* GetRenderPipelineState()
    {
        return m_RenderPipelineState;
    }
    
private:
    MTL::Device* m_MetalDevice;
    MTL::Library* m_Library;
    MTL::Function* m_VertexFunction;
    MTL::Function* m_FragmentFunction;
    MTL::RenderPipelineDescriptor* m_RenderPipelineDescriptor;
    MTL::RenderPipelineState* m_RenderPipelineState;
    MTL::DepthStencilState* m_DepthStencilState;
    MTL::DepthStencilDescriptor* m_DepthStencilDescriptor;
    MTL::VertexDescriptor* m_VertexDescriptor;
    MTL::RenderPipelineColorAttachmentDescriptor* m_ColorAttachmentDescriptor;
    MTL::PixelFormat m_DepthAttachmentPixelFormat;
    std::string m_FilePath;
    bool b_Result;
    
    
    std::string LoadShaderFile(const std::string& path);
    
    bool b_IsValid;
};



#endif //METALSHADER_H
