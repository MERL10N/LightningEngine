//
//  Renderer.hpp
//  LightningEngine
//
//  Created by Kian Marvi on 6/26/24.
//

#ifndef Renderer_h
#define Renderer_h

#include <DesignPatterns/SingletonTemplate.h>


namespace MTL
{
    class Device;
    class Library;
    class CommandQueue;
    class CommandBuffer;
    class RenderPipelineState;
    class Buffer;
    class ArgumentEncoder;
    class RenderCommandEncoder;
    class RenderPassDescriptor;
}

namespace NS
{
    class AutoreleasePool;
}

namespace MTK
{
class View;
}

class CRenderer : public CSingletonTemplate<CRenderer>
{
    friend CSingletonTemplate<CRenderer>;
public:
    CRenderer(MTL::Device* pDevice);
    ~CRenderer();
    void Draw(MTK::View* view);

private:
    
    void CreateSquare();
    
    MTL::Device* metalDevice;
    
    NS::AutoreleasePool* autoReleasePool;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* squareVertexBuffer;
    MTL::RenderCommandEncoder* renderCommandEncoder;
    MTL::RenderPassDescriptor* renderPassDescriptor;
};

#endif /* Renderer_hpp */
