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
    enum class RENDER_API
    {
        API_METAL = 0,
        API_VULKAN,
        NONE
    };

RENDER_API renderAPI = NONE;
public:
    Renderer() = default;

    template<typename RenderDevice>
    void Init(RenderDevice* renderDevice)
    {
        switch (renderAPI)
        {
            case API_METAL:
              static_cast<Derived*>(this)->InitMetal(renderDevice);
              break;
            case API_VULKAN:
                static_cast<Derived*>(this)->InitVulkan(renderDevice);
                break;
        default:
            throw std::runtime_error("Renderer::Init: Invalid render API");
        }
    }
    
    void Render()
    {
        static_cast<Derived*>(this)->Render(auto *view);
    }
    
    void Clean()
    {
        switch (renderAPI)
        {
        case RENDER_API::API_METAL:
            static_cast<Derived*>(this)->CleanMetal();
            break;
        case RENDER_API::API_VULKAN:
            static_cast<Derived*>(this)->CleanVulkan();
            break;
        default:
            throw std::runtime_error("Renderer::Render: Invalid render API");
        }
    }
};

#endif /* Renderer_hpp */
