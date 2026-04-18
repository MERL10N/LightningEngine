//
// Created by Kian Marvi on 12/14/24.
//

#include "MetalShader.h"
#include "Metal/Metal.hpp"
#include <iostream>
#include <fstream>
#include <string>


const std::string MetalShader::LoadShaderFile(const std::string &path) const
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
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
  m_DepthAttachmentPixelFormat(p_DepthAttachmentPixelFormat),
  m_RenderPipelineDescriptor(MTL::RenderPipelineDescriptor::alloc()->init())
{
    
    assert(m_MetalDevice);
    std::string shaderSrc = LoadShaderFile(m_FilePath);
    if (shaderSrc.empty())
    {
        std::cerr << "Error: metal shader is empty" << std::endl;
        return;
    }
    
    NS::Error* error = nullptr;
    
    m_Library = m_MetalDevice->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if (!m_Library)
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    m_VertexFunction = m_Library->newFunction(NS::String::string("vertex_main", NS::UTF8StringEncoding)); // Load the vertex function
    
    if (!m_VertexFunction)
    {
        std::cerr << "Error: Wrong name used for vertex shader function or is not found." << std::endl;
        std::cerr << "Error: Make sure your vertex shader name is: vertexShader" << std::endl;
    }
    else
    {
        std::cout << "Vertex function successfully found and loaded" << std::endl;
    }
    
    m_FragmentFunction = m_Library->newFunction(NS::String::string("fragment_main", NS::UTF8StringEncoding)); // Load the fragment function
    
    if (!m_FragmentFunction)
    {
        std::cerr << "Error: Wrong name used for fragmentShader function or is not found" << std::endl;
        std::cerr << "Error: Make sure your fragment shader name is: fragmentShader" << std::endl;
    }
    else
    {
        std::println("Fragment function successfully found and loaded");
    }


    m_RenderPipelineDescriptor->setRasterSampleCount(4);
    m_RenderPipelineDescriptor->setVertexFunction(m_VertexFunction);
    m_RenderPipelineDescriptor->setFragmentFunction(m_FragmentFunction);
    m_RenderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    m_RenderPipelineDescriptor->setDepthAttachmentPixelFormat(m_DepthAttachmentPixelFormat);
    
    assert(m_RenderPipelineDescriptor);
    
    m_ColorAttachmentDescriptor = m_RenderPipelineDescriptor->colorAttachments()->object(0);
    m_ColorAttachmentDescriptor->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    m_ColorAttachmentDescriptor->setBlendingEnabled(true);
    m_ColorAttachmentDescriptor->setRgbBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setAlphaBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);


    m_RenderPipelineDescriptor->setVertexDescriptor(m_VertexDescriptor);
    
    m_RenderPipelineState = m_MetalDevice->newRenderPipelineState(m_RenderPipelineDescriptor, &error);
    
    assert(m_RenderPipelineState);
    
    m_Library->release();
    m_RenderPipelineDescriptor->release();

}

MetalShader::MetalShader(const std::string &p_FilePath, const char* p_VertexFunction, const char* p_FragmentFunction, MTL::Device* p_MetalDevice, MTL::VertexDescriptor* p_VertexDescriptor, MTL::PixelFormat p_DepthAttachmentPixelFormat)
: m_MetalDevice(p_MetalDevice),
  m_FilePath(p_FilePath),
  m_DepthAttachmentPixelFormat(p_DepthAttachmentPixelFormat)
{
    
    assert(m_MetalDevice);
    std::string shaderSrc = LoadShaderFile(m_FilePath);
    if (shaderSrc.empty())
    {
        std::cerr << "Error: metal shader is empty" << std::endl;
        return;
    }
    
    NS::Error* error = nullptr;
    
    m_Library = m_MetalDevice->newLibrary(NS::String::string(shaderSrc.c_str(), NS::UTF8StringEncoding), nullptr, &error);
    if (!m_Library)
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    m_VertexFunction = m_Library->newFunction(NS::String::string(p_VertexFunction, NS::UTF8StringEncoding)); // Load the vertex function
    
    if (!m_VertexFunction)
    {
        std::cerr << "Error: Wrong name used for vertex shader function or is not found." << std::endl;
        std::cerr << "Error: Make sure your vertex shader name is: vertexShader" << std::endl;
    }
    
    m_FragmentFunction = m_Library->newFunction(NS::String::string(p_FragmentFunction, NS::UTF8StringEncoding)); // Load the fragment function
    
    if (!m_FragmentFunction)
    {
        std::cerr << "Error: Wrong name used for fragmentShader function or is not found" << std::endl;
        std::cerr << "Error: Make sure your fragment shader name is: fragmentShader" << std::endl;
    }
    
    if (m_VertexFunction && m_FragmentFunction)
    {
        std::println("Loading shader at: {}", m_FilePath);
    }

    m_RenderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    m_RenderPipelineDescriptor->setRasterSampleCount(4);
    m_RenderPipelineDescriptor->setVertexFunction(m_VertexFunction);
    m_RenderPipelineDescriptor->setFragmentFunction(m_FragmentFunction);
    m_RenderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(m_DepthAttachmentPixelFormat);
    m_RenderPipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
    
    assert(m_RenderPipelineDescriptor);
    
    m_ColorAttachmentDescriptor = m_RenderPipelineDescriptor->colorAttachments()->object(0);
    m_ColorAttachmentDescriptor->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

    m_ColorAttachmentDescriptor->setBlendingEnabled(true);
    m_ColorAttachmentDescriptor->setRgbBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setAlphaBlendOperation(MTL::BlendOperationAdd);
    m_ColorAttachmentDescriptor->setSourceRGBBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setSourceAlphaBlendFactor(MTL::BlendFactorSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationRGBBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);
    m_ColorAttachmentDescriptor->setDestinationAlphaBlendFactor(MTL::BlendFactorOneMinusSourceAlpha);


    m_RenderPipelineDescriptor->setVertexDescriptor(p_VertexDescriptor);
    
    m_RenderPipelineState = m_MetalDevice->newRenderPipelineState(m_RenderPipelineDescriptor, &error);
    
    assert(m_RenderPipelineState);
    
    m_Library->release();
    m_RenderPipelineDescriptor->release();

}

MetalShader::~MetalShader()
{
    std::println("Delete shader at: {}", m_FilePath);
    if (m_RenderPipelineState)
    {
        m_RenderPipelineState->release();
        m_RenderPipelineState = nullptr;
    }
    if (m_VertexFunction)
    {
        m_VertexFunction->release();
        m_VertexFunction = nullptr;
    }
    
    if (m_FragmentFunction)
    {
        m_FragmentFunction->release();
        m_FragmentFunction = nullptr;
    }
}
MTL::Buffer* MetalShader::InitialiseArgumentBuffers(const MTL::Texture* p_Texture) const
{
    MTL::ArgumentEncoder* m_ArgumentEncoder = m_FragmentFunction->newArgumentEncoder(0);
    MTL::Buffer* m_ArgumentBuffer = m_MetalDevice->newBuffer(m_ArgumentEncoder->encodedLength(), MTL::ResourceStorageModeManaged);
    
    m_ArgumentEncoder->setArgumentBuffer(m_ArgumentBuffer, 0);
    m_ArgumentEncoder->setTexture(p_Texture, 0);
    
    m_ArgumentEncoder->release();
    
    return m_ArgumentBuffer;
}

