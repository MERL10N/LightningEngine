//
// Created by Kian Marvi on 12/14/24.
//

#include "MetalShader.h"

#include <Metal/Metal.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <../spdlog/include/spdlog/spdlog.h>
#include "../../Logging/Log.h"

std::string MetalShader::LoadShaderFile(const std::string &path)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        Log::GetCoreLogger()->error("Cannot Open shader file");
        return "";
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return std::string(buffer.begin(), buffer.end());
}

MetalShader::MetalShader(const std::string& p_FilePath, MTL::Device* p_MetalDevice, MTL::PixelFormat p_DepthAttachmentPixelFormat)
: m_MetalDevice(p_MetalDevice),
  m_FilePath(p_FilePath),
  m_DepthAttachmentPixelFormat(p_DepthAttachmentPixelFormat)
{
    if (!p_MetalDevice)
    {
        Log::GetCoreLogger()->error("Metal is not supported on this device.");
        return;
    }

    std::string shaderSrc = LoadShaderFile(m_FilePath);
    if (shaderSrc.empty())
    {
        Log::GetCoreLogger()->error("Metal Shader is empty");
        std::cerr << "Error: metal shader is empty" << std::endl;
        return;
    }
    
    NS::Error* error = nullptr;
    
    m_Library = p_MetalDevice->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if (!m_Library)
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    m_VertexFunction = m_Library->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)); // Load the vertex function
    
    if (!m_VertexFunction)
    {
        std::cerr << "Error: Wrong name used for vertex shader function or is not found." << std::endl;
        std::cerr << "Error: Make sure your vertex shader name is: vertexShader" << std::endl;
    }
    else
    {
        std::cout << "Vertex function successfully found and loaded" << std::endl;
    }
    
    m_FragmentFunction = m_Library->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)); // Load the fragment function
    
    if (!m_FragmentFunction)
    {
        std::cerr << "Error: Wrong name used for fragmentShader function or is not found" << std::endl;
        std::cerr << "Error: Make sure your fragment shader name is: fragmentShader" << std::endl;
    }
    else
    {
        std::println("Fragment function successfully found and loaded");
    }
    
    b_Result = true;

    m_RenderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    m_RenderPipelineDescriptor->setVertexFunction(m_VertexFunction);
    m_RenderPipelineDescriptor->setFragmentFunction(m_FragmentFunction);
    m_RenderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    m_RenderPipelineDescriptor->setDepthAttachmentPixelFormat(m_DepthAttachmentPixelFormat);
    
    assert(m_RenderPipelineDescriptor);
    
    m_ColorAttachmentDescriptor = m_RenderPipelineDescriptor->colorAttachments()->object(0);
    m_ColorAttachmentDescriptor->setPixelFormat(MTL::PixelFormatRGBA8Unorm);

    m_ColorAttachmentDescriptor->setBlendingEnabled(true);
    m_ColorAttachmentDescriptor->setRgbBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setAlphaBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);


    // Initialise vertex descriptor
    m_VertexDescriptor = MTL::VertexDescriptor::vertexDescriptor();
    // Set attribute 0: position (float3 = 3 floats)
    m_VertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
    m_VertexDescriptor->attributes()->object(0)->setOffset(0);
    m_VertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    
    
    // Set attribute 1: color
    m_VertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);
    m_VertexDescriptor->attributes()->object(1)->setOffset(sizeof(float) * 3);
    m_VertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    
    // Set Attribute 2: Texture
    m_VertexDescriptor->attributes()->object(2)->setFormat(MTL::VertexFormatFloat3);
    m_VertexDescriptor->attributes()->object(2)->setOffset(sizeof(float) * 6);
    m_VertexDescriptor->attributes()->object(2)->setBufferIndex(0);

    // Set layout for buffer 0
    m_VertexDescriptor->layouts()->object(0)->setStride(sizeof(float) * 8);
    m_VertexDescriptor->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);
    
    assert(m_VertexDescriptor);

    m_RenderPipelineDescriptor->setVertexDescriptor(m_VertexDescriptor);
    
    m_RenderPipelineState = p_MetalDevice->newRenderPipelineState(m_RenderPipelineDescriptor, &error);
    
    if (!m_RenderPipelineState)
    {
        Log::GetCoreLogger()->error("Error occured when creating render pipeline state: ", error->localizedDescription()->utf8String());
    }
    assert(m_RenderPipelineState);
    
    m_Library->release();
    m_VertexFunction->release();
    m_FragmentFunction->release();

}

MetalShader::MetalShader(const std::string &p_FilePath, MTL::Device* p_MetalDevice, const char* p_VertexFunction, const char* p_FragmentFunction)
: m_MetalDevice(p_MetalDevice),
  m_FilePath(p_FilePath)
{
    if (!p_MetalDevice)
    {
        std::cerr << "Error: Metal is not supported on this device.\n";
        return;
    }

    std::string shaderSrc = LoadShaderFile(m_FilePath);
    if (shaderSrc.empty())
    {
        std::cerr << "Error: metal shader is empty" << std::endl;
        return;
    }
    
    NS::Error* error = nullptr;
    
    m_Library = p_MetalDevice->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if (!m_Library)
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    m_VertexFunction = m_Library->newFunction(NS::String::string(p_VertexFunction, NS::UTF8StringEncoding)); // Load the vertex function
    
    if (!m_VertexFunction)
    {
        std::cerr << "Error: Wrong name used for vertex shader function or is not found." << std::endl;
    }
    else
    {
        std::cout << "Vertex function successfully found and loaded" << std::endl;
    }
    
    m_FragmentFunction = m_Library->newFunction(NS::String::string(p_FragmentFunction, NS::UTF8StringEncoding)); // Load the fragment function
    
    if (!m_FragmentFunction)
    {
        std::cerr << "Error: Wrong name used for fragmentShader function or is not found" << std::endl;
    }
    else
    {
        std::println("Fragment function successfully found and loaded");
    }
    
    b_Result = true;

    m_RenderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    m_RenderPipelineDescriptor->setVertexFunction(m_VertexFunction);
    m_RenderPipelineDescriptor->setFragmentFunction(m_FragmentFunction);
    m_RenderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    
    m_RenderPipelineState = p_MetalDevice->newRenderPipelineState(m_RenderPipelineDescriptor, &error);
    
    if (!m_RenderPipelineState)
    {
        std::cerr << "Error occured when creating render pipeline state: " << error->localizedDescription()->utf8String() << std::endl;
    }
    assert(m_RenderPipelineState);
    
    m_Library->release();
    m_VertexFunction->release();
    m_FragmentFunction->release();

}

MetalShader::~MetalShader()
{
    m_RenderPipelineState->release();
    m_VertexDescriptor->release();
    m_RenderPipelineDescriptor->release();
    m_ColorAttachmentDescriptor->release();
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
