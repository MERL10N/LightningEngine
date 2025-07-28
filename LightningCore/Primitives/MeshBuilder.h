//
//  MeshBuilder.hpp
//  LightningCore
//
//  Created by Kian Marvi on 5/6/25.
//

#ifndef MESHBUILDER_H
#define MESHBUILDER_H

class SpriteAnimation;

class MeshBuilder
{
public:
    
    static void GenerateTriangle(void* p_VertexBuffer);
    static void GenerateQuad(void* p_VertexBuffer);
    
private:
    
};
#endif /* MESHBUILDER_H */
