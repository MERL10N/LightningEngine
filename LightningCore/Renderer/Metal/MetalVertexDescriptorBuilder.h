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

class MetalVertexDescriptorBuilder
{
public:
    MetalVertexDescriptorBuilder();
    ~MetalVertexDescriptorBuilder();
    
    MetalVertexDescriptorBuilder& AddAttribute(MTL::VertexFormat vertexFormat, uint16_t offset, uint16_t index = 0);
    
    MetalVertexDescriptorBuilder& SetBufferLayout(uint16_t stride, MTL::VertexStepFunction stepFunction = MTL::VertexStepFunctionPerVertex, uint16_t index = 0);
    
    MTL::VertexDescriptor* BuildVertexDescriptor();
    
    void Reset();
private:
    MTL::VertexDescriptor* m_VertexDescriptor = nullptr;
    uint16_t m_CurrentAttributeIndex = 0;
};

#endif /* MetalVertexDescriptorBuilder_h */
