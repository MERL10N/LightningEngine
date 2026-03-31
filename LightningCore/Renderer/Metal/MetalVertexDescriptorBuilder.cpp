//
//  MetalVertexDescriptorBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 3/31/26.
//

#include "MetalVertexDescriptorBuilder.h"

MetalVertexDescriptorBuilder::MetalVertexDescriptorBuilder()
: m_VertexDescriptor(MTL::VertexDescriptor::alloc()->init()),
  m_CurrentAttributeIndex(0)
{
}

MetalVertexDescriptorBuilder::~MetalVertexDescriptorBuilder()
{
    if (m_VertexDescriptor)
    {
        m_VertexDescriptor->release();
        m_VertexDescriptor = nullptr;
    }
}

void MetalVertexDescriptorBuilder::Reset()
{
    if (m_VertexDescriptor)
    {
        m_VertexDescriptor->release();
        m_VertexDescriptor = nullptr;
    }
    
    m_CurrentAttributeIndex = 0;
    m_VertexDescriptor = MTL::VertexDescriptor::alloc()->init();
}

MetalVertexDescriptorBuilder &MetalVertexDescriptorBuilder::AddAttribute(MTL::VertexFormat vertexFormat, uint16_t offset, uint16_t index)
{
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setFormat(vertexFormat);
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setOffset(offset);
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setBufferIndex(index);
    ++m_CurrentAttributeIndex;
    return *this;
}


MetalVertexDescriptorBuilder &MetalVertexDescriptorBuilder::SetBufferLayout(uint16_t stride, MTL::VertexStepFunction stepFunction, uint16_t index)
{
    m_VertexDescriptor->layouts()->object(0)->setStride(stride);
    m_VertexDescriptor->layouts()->object(0)->setStepFunction(stepFunction);
    
    return *this;
}

MTL::VertexDescriptor* MetalVertexDescriptorBuilder::BuildVertexDescriptor()
{
    MTL::VertexDescriptor* builtDescriptor = m_VertexDescriptor;
    
    m_VertexDescriptor = nullptr;
    
    Reset();
    
    return builtDescriptor;
}
    
    
