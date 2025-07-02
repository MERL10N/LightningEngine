//
// Created by Kian Marvi on 12/14/24.
//

#ifndef METALSHADER_H
#define METALSHADER_H

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
    MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice);
    ~MetalShader();
    
    template <typename T>
    void SetFragmentShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    template <typename T>
    void SetVertexShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    MTL::RenderPipelineState* GetRenderPipelineState();
    
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
    MTL::RenderPipelineColorAttachmentDescriptor* colorAttachmentDescriptor;
    std::string m_FilePath;
    bool b_Result;
    
    
    std::string LoadShaderFile(const std::string& path);
    
    bool b_IsValid;
};



#endif //METALSHADER_H
