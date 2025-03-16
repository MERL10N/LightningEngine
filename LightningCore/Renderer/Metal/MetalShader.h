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
}

class MetalShader
{
    public:
    MetalShader(const std::string &filePath, MTL::Device* device);
    ~MetalShader();
    
    template <typename T>
    void SetFragmentShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    template <typename T>
    void SetVertexShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index);
    
    MTL::RenderPipelineState* GetRenderPSO();
    
    private:
    MTL::Device* device;
    MTL::Library* library;
    MTL::Function* vertexFunction;
    MTL::Function* fragmentFunction;
    MTL::RenderPipelineDescriptor* renderPipelineDescriptor;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::DepthStencilState* depthStencilState;
    MTL::DepthStencilDescriptor* depthStencilDescriptor;
    MTL::Texture* targetTexture;
    std::string filePath;
    bool bResult;
    
    
    std::string LoadShaderFile(const std::string& path);
    
    bool isValid;
};



#endif //METALSHADER_H
