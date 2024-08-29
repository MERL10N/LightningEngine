/*
    MeshBuilder.hpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp

namespace MTL
{
    class Buffer;
    class Device;
}

class MeshBuilder
{
public:
    // Generate a quad
    static MTL::Buffer* GenerateQuad(MTL::Device* metalDevice);
    static MTL::Buffer* GenerateCube();
};

#endif /* MeshBuilder_hpp */
