//
//  Renderer.h
//  LightningEngine
//
//  Created by Kian Marvi on 1/6/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <stdexcept>

template <class RenderAPI>
class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    template<typename RenderView>
    void Render(RenderView* renderView)
    {
        static_cast<RenderAPI*>(this)->Render(renderView);
    }
    
};

#endif /* Renderer_hpp */
