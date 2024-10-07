/**
     file: MetalShader,h
     @brief This class stores a metal shader object, using metal-cpp as backend
     By: Kian Heydari Marvi
     Date: Jan 2024
*/

#ifndef METAL_SHADER_H
#define METAL_SHADER_H
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>


class MetalShader
{
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

    // Shader name
    std::string name;

    std::string LoadShaderFile(const std::string& path);

public:
    MetalShader(const std::string& path, MTK::View* view);
        
    inline void SetRenderPipelineState(MTL::RenderPipelineState* metalRenderPSO)
    {
        this->metalRenderPSO = metalRenderPSO;
    }
    
    inline MTL::RenderPipelineDescriptor* GetRenderPipelineDescriptor()
    {
        return renderPipelineDescriptor;
    }
    
    inline MTL::RenderPipelineState* GetRenderPipelineState()
    {
        return metalRenderPSO;
    }
    
    inline MTL::DepthStencilState* GetDepthStencilState()
    {
        return depthStencilState;
    }
    
    inline void BindResources(MTL::RenderCommandEncoder* encoder, MTL::Buffer* buffer)
    {
        encoder->useResource(buffer, MTL::ResourceUsageRead);
        encoder->setVertexBuffer(buffer, 0, 0); // Bind as a vertex buffer
    }
    
    void SetName(const std::string name)
    {
        this->name = name;
    }
    
    void use(MTL::RenderCommandEncoder* commandEncoder)
    {
        commandEncoder->setRenderPipelineState(GetRenderPipelineState());
    }
    // Get shader name
// ------------------------------------------------------------------------
    std::string& GetName(void)
    {
        return name;
    }
    
    bool IsLoaded(void) const
    {
        return bResult;
    }

    ~MetalShader()
    {
    }
};

#endif
