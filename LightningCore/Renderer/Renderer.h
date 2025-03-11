//
//  Renderer.h
//  LightningEngine
//
//  Created by Kian Marvi on 1/6/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <stdexcept>

template <class Derived>
class Renderer
{
public:
    Renderer() = default;

    template<typename RenderDevice>
    void Init(RenderDevice* renderDevice)
    {
        static_cast<Derived*>(this)->Initialise(renderDevice);
    }
    
    template<typename RenderView>
    void Render(RenderView* renderView)
    {
        static_cast<Derived*>(this)->Draw(renderView);
    }
    
    void Clean()
    {
        static_cast<Derived*>(this)->Destroy();
    }
};

#endif /* Renderer_hpp */
