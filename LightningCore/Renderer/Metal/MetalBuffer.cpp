//
//  MetalBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MetalBuffer.h"
#include <Metal/Metal.hpp>


MetalVertexBuffer::MetalVertexBuffer(MTL::Device* p_MetalDevice)
: m_MetalDevice(p_MetalDevice)
{
}

void MetalVertexBuffer::BindBuffer(const float* p_Vertices, uint32_t p_Size, MTL::RenderCommandEncoder* p_Encoder)
{
    assert(m_MetalDevice);
    assert(p_Encoder);
    if (!m_VertexBuffer)
    {
        m_VertexBuffer = m_MetalDevice->newBuffer(p_Vertices, p_Size, MTL::ResourceStorageModeManaged);
    }

    p_Encoder->setVertexBuffer(m_VertexBuffer, 0, 0);
    
}

MetalVertexBuffer::~MetalVertexBuffer()
{
   if (m_VertexBuffer)
   {
       m_VertexBuffer->release();
   }
   if (m_MetalDevice)
   {
       m_MetalDevice->release();
   }
}

