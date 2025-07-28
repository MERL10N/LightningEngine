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
    void BindBuffer(const float* p_Vertices, uint32_t p_Size);
    
    inline MTL::Buffer* GetVertexBuffer() { return m_VertexBuffer; }
    
private:
    MTL::Buffer* m_VertexBuffer;
    MTL::Device* m_MetalDevice;
    bool b_IsSet;
    
};

#endif /* MetalBuffer_hpp */
