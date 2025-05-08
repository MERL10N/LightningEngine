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

    MetalVertexBuffer(const float* p_Vertices, uint32_t p_Size, MTL::Device* p_MetalDevice);
    ~MetalVertexBuffer();
    
    MTL::Buffer* GetVertexBuffer();
    
private:
    MTL::Buffer* m_VertexBuffer;
    
};

#endif /* MetalBuffer_hpp */
