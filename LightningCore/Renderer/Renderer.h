//
//  Renderer.h
//  LightningEngine
//
//  Created by Kian Marvi on 1/6/25.
//

#ifndef RENDERER_H
#define RENDERER_H

template <typename Derived>
class Renderer
{
public:
    Renderer() = default;
    
    void Init();
    
    void Render();
    
    void Clean();
};

#endif /* Renderer_hpp */
