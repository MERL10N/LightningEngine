//
//  Layer.h
//  LightningCore
//
//  Created by Kian Marvi on 7/9/25.
//

#ifndef LAYER_H
#define LAYER_H

class Layer
{
public:
    Layer(const char* name = "Layer");
    
    virtual ~Layer();
    
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}

    
    inline const char* GetName() const { return m_DebugName; }
    
protected:
    const char* m_DebugName;
    
};

#endif /* Layer_hpp */
