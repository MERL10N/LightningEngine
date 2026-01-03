//
// Created by Kian Marvi on 12/14/24.
//

#include "MetalShader.h"
#include "Metal/Metal.hpp"
#include <iostream>
#include <fstream>
#include <string>


std::string MetalShader::LoadShaderFile(const std::string &path)
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

    m_RenderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    m_RenderPipelineDescriptor->setVertexFunction(m_VertexFunction);
    m_RenderPipelineDescriptor->setFragmentFunction(m_FragmentFunction);
    m_RenderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(m_DepthAttachmentPixelFormat);
    m_RenderPipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);
    
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

    NS::UInteger offset = 0;
    
    m_VertexDescriptor = MTL::VertexDescriptor::alloc()->init();
    
    // Set attribute 0: position (vec3)
    m_VertexDescriptor->attributes()->object(0)->setFormat(MTL::VertexFormatFloat3);
    m_VertexDescriptor->attributes()->object(0)->setOffset(offset);
    m_VertexDescriptor->attributes()->object(0)->setBufferIndex(0);
    
    offset += 4 * sizeof(float);
    
    // Set attribute 1: color (vec3)
    m_VertexDescriptor->attributes()->object(1)->setFormat(MTL::VertexFormatFloat3);
    m_VertexDescriptor->attributes()->object(1)->setOffset(offset);
    m_VertexDescriptor->attributes()->object(1)->setBufferIndex(0);
    
    offset += 4 * sizeof(float);
    
    // Set Attribute 2: Texture (vec2)
    m_VertexDescriptor->attributes()->object(2)->setFormat(MTL::VertexFormatFloat2);
    m_VertexDescriptor->attributes()->object(2)->setOffset(offset);
    m_VertexDescriptor->attributes()->object(2)->setBufferIndex(0);
    
    offset += 4 * sizeof(float);

    // Set layout for buffer 0
    m_VertexDescriptor->layouts()->object(0)->setStride(offset);
    m_VertexDescriptor->layouts()->object(0)->setStepFunction(MTL::VertexStepFunctionPerVertex);
    
    assert(m_VertexDescriptor);

    m_RenderPipelineDescriptor->setVertexDescriptor(m_VertexDescriptor);
    
    m_RenderPipelineState = p_MetalDevice->newRenderPipelineState(m_RenderPipelineDescriptor, &error);
    
    assert(m_RenderPipelineState);
    
    m_Library->release();
    m_VertexFunction->release();
    m_FragmentFunction->release();
    m_RenderPipelineDescriptor->release();

}

MetalShader::~MetalShader()
{
    m_RenderPipelineState->release();
    m_VertexDescriptor->release();
}

void MetalShader::SetDevice(MTL::Device *p_MetalDevice)
{
    m_MetalDevice = p_MetalDevice;
}
