//
//  MeshBuilder.h
//  LightningCore
//
//  Created by Kian Marvi on 3/13/25.
//

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp

#include <stdio.h>

namespace MTL
{
    class Buffer;
    class Device;
    class RenderCommandEncoder;
}

class MetalShader;

class MeshBuilder
{
public:
    // Generate a quad
    static MTL::Buffer* GenerateQuad(MTL::Device* metalDevice);
    static MTL::Buffer* GenerateCube(MTL::Device* metalDevice);
    static MTL::Buffer* GenerateTriangle(MTL::Device* metalDevice);
    
    void GenerateTriangle(MTL::RenderCommandEncoder* encoder, MetalShader& shader);
};


#endif /* MeshBuilder_hpp */
