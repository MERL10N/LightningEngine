//
//  MetalBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MetalBuffer.h"
#include "Metal/Metal.hpp"

MetalVertexBuffer::MetalVertexBuffer(MTL::Device* p_MetalDevice)
: m_MetalDevice(p_MetalDevice),
  m_VertexBuffer(nullptr)
{
}

void MetalVertexBuffer::BindBuffer(const void* p_Vertices)
{
    assert(m_MetalDevice);
    
    if (!m_VertexBuffer)
    {
        m_VertexBuffer = m_MetalDevice->newBuffer(p_Vertices, sizeof(p_Vertices), MTL::ResourceStorageModeShared);
    }
    
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



MetalIndexBuffer::MetalIndexBuffer(MTL::Device *p_MetalDevice)
: m_MetalDevice(p_MetalDevice),
  m_IndexBuffer(nullptr)
{
}
