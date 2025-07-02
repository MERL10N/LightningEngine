//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp

class SpriteAnimation;

class MeshBuilder
{
public:
    static void GenerateTriangle(void* p_Device, void* p_VertexBuffer);
    
    static void GenerateQuad(void* p_Device, void* p_VertexBuffer);
    
    
    
private:
    
};
#endif /* MeshBuilder_hpp */
