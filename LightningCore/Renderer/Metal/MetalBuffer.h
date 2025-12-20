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
    MetalVertexBuffer(MTL::Device* p_MetalDevice);
    ~MetalVertexBuffer();
        
    void BindBuffer(const void* p_Vertices);
    
    inline MTL::Buffer* GetVertexBuffer() { return m_VertexBuffer; }
    
private:
    MTL::Buffer* m_VertexBuffer = nullptr;
    MTL::Device* m_MetalDevice = nullptr;
};

class MetalIndexBuffer
{
public:
    MetalIndexBuffer() = default;
    MetalIndexBuffer(MTL::Device* p_MetalDevice);
    ~MetalIndexBuffer();
        
    void BindBuffer(const float* p_Vertices);
    
    inline MTL::Buffer* GetIndexBuffer() { return m_IndexBuffer; }
    
private:
    MTL::Buffer* m_IndexBuffer = nullptr;
    MTL::Device* m_MetalDevice = nullptr;
};

#endif /* MetalBuffer_hpp */
