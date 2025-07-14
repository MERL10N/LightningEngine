//
//  LayerStack.h
//  LightningCore
//  Based on Cherno Game Engine Series Tutorial
//  Created by Kian Marvi on 7/9/25.
//

#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include <vector>

class Layer;

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();
    
    void PushLayer(Layer* p_Layer);
    void PushOverlay(Layer* p_Overlay);
    
    void PopLayer(Layer* p_Layer);
    void PopOverlay(Layer* p_Overlay);
    
    std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_Layers.end(); }
private:
    std::vector<Layer*> m_Layers;
    std::vector<Layer*>::iterator m_LayerInsert;
};

#endif /* LayerStack_hpp */
