//
// Created by Kian Marvi on 11/24/24.
//

#ifndef METALKITVIEWDELEGATE_H
#define METALKITVIEWDELEGATE_H

#include <MetalKit/MTKView.hpp>

#include "../../Renderer/Renderer.h"

class MetalRenderer;


class MetalKitViewDelegate : public MTK::ViewDelegate
{
public:
    explicit MetalKitViewDelegate( MTL::Device* metalDevice );
    ~MetalKitViewDelegate() override;
    void drawInMTKView( MTK::View* metalKitView ) override;

private:
    Renderer<MetalRenderer>* metalRenderer;
};



#endif //METALKITVIEWDELEGATE_H
