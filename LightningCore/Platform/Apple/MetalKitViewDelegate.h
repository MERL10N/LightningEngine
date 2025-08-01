//
// Created by Kian Marvi on 11/24/24.
//

#ifndef METALKITVIEWDELEGATE_H
#define METALKITVIEWDELEGATE_H

#include <MetalKit/MTKView.hpp>

#include "../../Renderer/Metal/MetalRenderer.h"

class MetalKitViewDelegate : public MTK::ViewDelegate
{
public:
    explicit MetalKitViewDelegate(MTK::View* p_MetalKitView );
    virtual ~MetalKitViewDelegate() override;
    virtual void drawInMTKView(MTK::View* p_MetalKitView ) override;
    
    inline MetalRenderer GetRenderer() { return m_MetalRenderer; }

private:
    MetalRenderer m_MetalRenderer;
};



#endif //METALKITVIEWDELEGATE_H
