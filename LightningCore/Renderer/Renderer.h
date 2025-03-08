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
        static_cast<Derived*>(this)->Initalise(renderDevice);
    }
    
    void Draw()
    {
        static_cast<Derived*>(this)->Render();
    }
    
    void Clean()
    {
        static_cast<Derived*>(this)->CleanRenderer(renderDevice);
    }
};

#endif /* Renderer_hpp */
