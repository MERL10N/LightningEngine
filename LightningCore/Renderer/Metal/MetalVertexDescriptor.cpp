//
//  MetalVertexDescriptorBuilder.cpp
//  LightningCore
//
//  Created by Kian Marvi on 3/31/26.
//

#include "MetalVertexDescriptor.h"

MetalVertexDescriptor::MetalVertexDescriptor()
: m_VertexDescriptor(MTL::VertexDescriptor::alloc()->init()),
  m_CurrentAttributeIndex(0)
{
}

MetalVertexDescriptor::~MetalVertexDescriptor()
{
    if (m_VertexDescriptor)
    {
        m_VertexDescriptor->release();
        m_VertexDescriptor = nullptr;
    }
}

void MetalVertexDescriptor::Reset()
{
    if (m_VertexDescriptor)
    {
        m_VertexDescriptor->release();
        m_VertexDescriptor = nullptr;
    }
    
    m_CurrentAttributeIndex = 0;
    m_VertexDescriptor = MTL::VertexDescriptor::alloc()->init();
}

MetalVertexDescriptor &MetalVertexDescriptor::AddAttribute(const MTL::VertexFormat vertexFormat, const uint16_t offset, const uint16_t index)
{
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setFormat(vertexFormat);
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setOffset(offset);
    m_VertexDescriptor->attributes()->object(m_CurrentAttributeIndex)->setBufferIndex(index);
    ++m_CurrentAttributeIndex;
    return *this;
}


MetalVertexDescriptor &MetalVertexDescriptor::SetBufferLayout(const uint16_t stride, const MTL::VertexStepFunction stepFunction, const uint16_t index)
{
    m_VertexDescriptor->layouts()->object(0)->setStride(stride);
    m_VertexDescriptor->layouts()->object(0)->setStepFunction(stepFunction);
    return *this;
}

MTL::VertexDescriptor* MetalVertexDescriptor::BuildVertexDescriptor()
{
    MTL::VertexDescriptor* builtDescriptor = m_VertexDescriptor;
    
    m_VertexDescriptor = nullptr;
    
    Reset();
    
    return builtDescriptor;
}
