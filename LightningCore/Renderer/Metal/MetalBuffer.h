//
//  MetalBuffer.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MetalBuffer_hpp
#define MetalBuffer_hpp

#include <simd/simd.h>

namespace MTL
{
    class Device;
    class Buffer;
    class RenderCommandEncoder;
}

class MetalVertexBuffer
{
public:
    MetalVertexBuffer() = default;
    MetalVertexBuffer(MTL::Device* p_MetalDevice, const uint32_t p_Size);
    ~MetalVertexBuffer();
    
    static MTL::Buffer* Create(MTL::Device* p_MetalDevice, const uint32_t size);
    
    inline const MTL::Buffer* GetVertexBuffer() const { return m_VertexBuffer; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Buffer* m_VertexBuffer = nullptr;
};

class MetalIndexBuffer
{
public:
    MetalIndexBuffer() = default;
    MetalIndexBuffer(MTL::Device* p_MetalDevice, const uint32_t* indices, const uint32_t size);
    ~MetalIndexBuffer();
    
    static MTL::Buffer* Create(MTL::Device* p_MetalDevice, const uint16_t* indices, const uint32_t size);
           
    inline const MTL::Buffer* GetIndexBuffer() const { return m_IndexBuffer; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Buffer* m_IndexBuffer = nullptr;
};

#endif /* MetalBuffer_hpp */
