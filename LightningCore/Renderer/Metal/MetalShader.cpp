//
// Created by Kian Marvi on 12/14/24.
//

#include "MetalShader.h"

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <iostream>
#include <fstream>
#include <string>


std::string MetalShader::LoadShaderFile(const std::string &path)
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

MetalShader::MetalShader(const std::string& path, MTL::Device* device)
: device(device),
  filePath(path)
{
    if (!device)
    {
        std::cerr << "Error: Metal is not supported on this device.\n";
        return;
    }

    std::string shaderSrc = LoadShaderFile(filePath);
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

    vertexFunction = library->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)); // Load the vertex function
    
    if (!vertexFunction)
    {
        std::cerr << "Error: Wrong name used for vertex shader function or is not found." << std::endl;
        std::cerr << "Error: Make sure your vertex shader name is: vertexShader" << std::endl;
    }
    else
    {
        std::cout << "Vertex function successfully found and loaded" << std::endl;
    }
    
    fragmentFunction = library->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)); // Load the fragment function
    
    if (!fragmentFunction)
    {
        std::cerr << "Error: Wrong name used for fragmentShader function or is not found" << std::endl;
        std::cerr << "Error: Make sure your fragment shader name is: fragmentShader" << std::endl;
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
    //renderPipelineDescriptor->setSampleCount(4);
    //renderPipelineDescriptor->setAtt(MTL::PixelFormatBGRA8Unorm);
    
    metalRenderPSO = device->newRenderPipelineState(renderPipelineDescriptor, &error);
    
    if (!metalRenderPSO)
    {
        std::cerr << "Error occured when creating render pipeline state: " << error->localizedDescription()->utf8String() << std::endl;
    }
    
   // depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
   // depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
    //depthStencilDescriptor->setDepthWriteEnabled(true);
   // depthStencilState = device->newDepthStencilState(depthStencilDescriptor);

    renderPipelineDescriptor->release();
   // depthStencilDescriptor->release();
    library->release();
    vertexFunction->release();
    fragmentFunction->release();

}

MetalShader::~MetalShader()
{
    metalRenderPSO->release();
}

template <typename T>
void MetalShader::SetFragmentShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index)
{
    encoder->setFragmentBytes(value, sizeof(value), index);
}

template <typename T>
void MetalShader::SetVertexShaderUniform(MTL::RenderCommandEncoder* encoder, const T& value, const int index)
{
    encoder->setVertexBytes(value, sizeof(value), index);
}

MTL::RenderPipelineState* MetalShader::GetRenderPSO()
{
    return metalRenderPSO;
}
