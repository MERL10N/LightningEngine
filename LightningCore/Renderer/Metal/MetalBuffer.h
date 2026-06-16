//
//  MetalBuffer.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MetalBuffer_hpp
#define MetalBuffer_hpp

#include <simd/simd.h>
#include "Metal/MTLDevice.hpp"

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
    MetalVertexBuffer(MTL::Device* p_MetalDevice);
    ~MetalVertexBuffer();
       
    template<typename T>
    void BindBuffer(T* p_Vertices, unsigned int p_ResourceStorageOption)
    {
        if (!m_VertexBuffer)
        {
            m_VertexBuffer = m_MetalDevice->newBuffer(p_Vertices, sizeof(p_Vertices), p_ResourceStorageOption);
        }
    }
    
    inline const MTL::Buffer* GetVertexBuffer() const { return m_VertexBuffer; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Buffer* m_VertexBuffer = nullptr;
};

class MetalIndexBuffer
{
public:
    MetalIndexBuffer() = default;
    MetalIndexBuffer(MTL::Device* p_MetalDevice);
    ~MetalIndexBuffer();
        
    void BindBuffer(const void* p_Vertices);
    
    inline const MTL::Buffer* GetIndexBuffer() const { return m_IndexBuffer; }
    
private:
    MTL::Device* m_MetalDevice = nullptr;
    MTL::Buffer* m_IndexBuffer = nullptr;
};

class MetalArgumentBuffer
{
public:
private:
    
};

#endif /* MetalBuffer_hpp */
