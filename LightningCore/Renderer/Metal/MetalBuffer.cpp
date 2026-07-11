//
//  MetalBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MetalBuffer.h"
#include "Metal/Metal.hpp"

MetalVertexBuffer::MetalVertexBuffer(MTL::Device* p_MetalDevice, const uint32_t p_Size)
: m_MetalDevice(p_MetalDevice)
{
    if (!m_VertexBuffer)
    {
        m_VertexBuffer = m_MetalDevice->newBuffer(nullptr, p_Size, MTL::ResourceStorageModeShared);
    }
}

MetalVertexBuffer::~MetalVertexBuffer()
{
   if (m_VertexBuffer)
   {
       m_VertexBuffer->release();
       m_VertexBuffer = nullptr;
   }
   if (m_MetalDevice)
   {
       m_MetalDevice->release();
       m_MetalDevice = nullptr;
   }
}

MTL::Buffer *MetalVertexBuffer::Create(MTL::Device *p_MetalDevice, const uint32_t size)
{
    return p_MetalDevice->newBuffer(size, MTL::ResourceStorageModeShared);
}


MetalIndexBuffer::MetalIndexBuffer(MTL::Device *p_MetalDevice, const uint32_t* indices, const uint32_t size)
: m_MetalDevice(p_MetalDevice)
{
    if (!m_IndexBuffer)
    {
        m_IndexBuffer = m_MetalDevice->newBuffer(indices, size, MTL::ResourceStorageModeShared);
    }
}

MetalIndexBuffer::~MetalIndexBuffer()
{
   if (m_IndexBuffer)
   {
       m_IndexBuffer->release();
   }
   if (m_MetalDevice)
   {
       m_MetalDevice->release();
   }
}

MTL::Buffer* MetalIndexBuffer::Create(MTL::Device* p_MetalDevice, const uint16_t* indices, const uint32_t size)
{
    return p_MetalDevice->newBuffer(indices, size, MTL::ResourceStorageModeShared);
}
