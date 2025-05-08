//
//  MetalBuffer.cpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#include "MetalBuffer.h"
#include <Metal/Metal.hpp>


MetalVertexBuffer::MetalVertexBuffer(const float* p_Vertices, uint32_t p_Size, MTL::Device* p_MetalDevice)
{
    m_VertexBuffer = p_MetalDevice->newBuffer(p_Vertices, p_Size, MTL::ResourceStorageModeManaged);
}

MTL::Buffer* MetalVertexBuffer::GetVertexBuffer()
{
    return m_VertexBuffer;
}

MetalVertexBuffer::~MetalVertexBuffer()
{
   if (m_VertexBuffer)
   {
       m_VertexBuffer->release();
   }
}

