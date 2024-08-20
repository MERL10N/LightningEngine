/**
     file: MetalShader,h
     @brief This class stores a metal shader object, using metal-cpp as backend
     By: Kian Heydari Marvi
     Date: Jan 2024
*/

#ifndef METAL_SHADER_H
#define METAL_SHADER_H
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <Editor/Editor.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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
    std::string filePath;
    bool bResult;

    // Shader name
    std::string name;

    std::string loadShaderFile(const std::string& path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open shader file." << std::endl;
            return "";
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return std::string(buffer.begin(), buffer.end());
    }

public:
    MetalShader(const std::string& path, const char* vertName, const char* fragName, MTL::Device* device)
        : filePath(path),
          device(device)
    {
        if (!device)
        {
            std::cerr << "Error: Metal is not supported on this device." << std::endl;
            return;
        }

        std::string shaderSrc = loadShaderFile(filePath);
        if (shaderSrc.empty())
        {
            std::cerr << "Error: metal shader is empty" << std::endl;
            return;
        }
        

        NS::Error* error = nullptr;
        library = device->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &error);
        if (!library)
        {
            __builtin_printf( "%s", error->localizedDescription()->utf8String() );
            assert( false );
        }

        vertexFunction = library->newFunction(NS::String::string(vertName, NS::UTF8StringEncoding)); // Load the vertex function
        
        if (!vertexFunction)
        {
            std::cerr << "Error: Vertex function not found in shader library." << std::endl;
        }
        else
        {
            std::cout << "Vertex function successfully found and loaded" << std::endl;
        }
        
        fragmentFunction = library->newFunction(NS::String::string(fragName, NS::UTF8StringEncoding)); // Load the fragment function
        
        if (!fragmentFunction)
        {
            std::cerr << "Error: Fragment function not found in shader library." << std::endl;
        }
        else
        {
            std::cout << "Fragment function successfully found and loaded" << std::endl;
        }
        
        bResult = true;

        renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
        renderPipelineDescriptor->setVertexFunction(vertexFunction);
        renderPipelineDescriptor->setFragmentFunction(fragmentFunction);
        assert(renderPipelineDescriptor);
        renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        renderPipelineDescriptor->setSampleCount(4);
        renderPipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
        SetRenderPipelineState(metalRenderPSO);
        metalRenderPSO = device->newRenderPipelineState(renderPipelineDescriptor, &error);
        
        if (!metalRenderPSO)
        {
            std::cerr << "Error occured when creating render pipeline state: " << error->localizedDescription()->utf8String() << std::endl;
        }
        
        MTL::DepthStencilDescriptor* depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
        depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
        depthStencilDescriptor->setDepthWriteEnabled(true);
        depthStencilState = device->newDepthStencilState(depthStencilDescriptor);

        renderPipelineDescriptor->release();
        depthStencilDescriptor->release();
        library->release();
        vertexFunction->release();
        fragmentFunction->release();
    }
    
    void SetRenderPipelineState(MTL::RenderPipelineState* metalRenderPSO)
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
    
    void BindResources(MTL::RenderCommandEncoder* encoder, MTL::Buffer* buffer)
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
