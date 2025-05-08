//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp
#ifdef __APPLE__
    class MetalVertexBuffer;
    using VertexBuffer = MetalVertexBuffer;
#endif
class MeshBuilder
{
public:
    static VertexBuffer* GenerateTriangle(void* p_Device);
};
#endif /* MeshBuilder_hpp */
