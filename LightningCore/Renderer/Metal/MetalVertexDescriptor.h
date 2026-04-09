//
//  MetalVertexDescriptorBuilder.h
//  LightningCore
//
//  Created by Kian Marvi on 3/31/26.
//

#ifndef MetalVertexDescriptorBuilder_hpp
#define MetalVertexDescriptorBuilder_hpp

#include <stdint.h>
#include "Metal/MTLVertexDescriptor.hpp"

class MetalVertexDescriptor
{
public:
    MetalVertexDescriptor();
    ~MetalVertexDescriptor();
    
    MetalVertexDescriptor& AddAttribute(const MTL::VertexFormat vertexFormat, const uint16_t offset, const uint16_t index = 0);
    
    MetalVertexDescriptor& SetBufferLayout(const uint16_t stride, const MTL::VertexStepFunction stepFunction = MTL::VertexStepFunctionPerVertex, const uint16_t index = 0);
    
    MTL::VertexDescriptor* BuildVertexDescriptor();
    
    void Reset();
private:
    MTL::VertexDescriptor* m_VertexDescriptor = nullptr;
    uint16_t m_CurrentAttributeIndex = 0;
};

#endif /* MetalVertexDescriptorBuilder_h */
