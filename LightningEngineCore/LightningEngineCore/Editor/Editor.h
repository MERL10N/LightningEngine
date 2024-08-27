//
//  Editor.h
//  LightningEngine
//
//  Created by Kian Marvi on 2/2/24.
//

#ifndef Editor_h
#define Editor_h

#include <DesignPatterns/SingletonTemplate.h>

namespace MTL
{
    class Device;
    class RenderPassDescriptor;
    class CommandBuffer;
    class RenderCommandEncoder;
}
struct ImFont;
namespace MTK
{
    class View;
}
class CEditor : public CSingletonTemplate<CEditor>
{
    friend CSingletonTemplate<CEditor>;
public:
    bool Init(MTL::Device* device, MTK::View * view);
    void Render(MTL::RenderPassDescriptor* renderPassDescriptor, MTL::CommandBuffer* metalCommandBuffer, MTL::RenderCommandEncoder* renderCommandEncoder, MTK::View * view);
    
    void RenderDockSpace();
    
   // Setters
   void SetClearColor(float value, int index);
   void SetFrameRate(int fps);
    
   // Getters
   float GetClearColor(int index) const;
   int   GetFrameRate();
    
   void Destroy();
    
private:
    bool show_demo_window;
    bool show_another_window;
    bool enable_high_fps;
    float clear_color[4] = {0.15f, 0.15f, 0.15f, 1.f};
    int fps;
    ImFont* mainFont;
};

#endif /* Editor_h */
