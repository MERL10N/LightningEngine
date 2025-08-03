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
    MTL::PixelFormat m_DepthAttachmentPixelFormat;
    std::string m_FilePath;
    bool b_Result;
    
    
    std::string LoadShaderFile(const std::string& path);
    
    bool b_IsValid;
};



#endif //METALSHADER_H
