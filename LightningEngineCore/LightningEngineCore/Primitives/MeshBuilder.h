/*
    MeshBuilder.hpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp

#include <Metal/Metal.hpp>

class CMeshBuilder
{
public:
    // Generate a quad
    static MTL::Buffer* GenerateQuad(MTL::Device* metalDevice);
    static MTL::Buffer* GenerateCube(MTL::Device* metalDevice);
};

#endif /* MeshBuilder_hpp */
