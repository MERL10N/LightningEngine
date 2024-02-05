/*
    MeshBuilder.hpp
    LightningEngine

    Created by Kian Marvi on 2/2/24.
*/

#ifndef MeshBuilder_hpp
#define MeshBuilder_hpp

#include "Mesh.h"
#include <Metal/Metal.hpp>

class CMeshBuilder
{
public:
    // Generate a quad
    static CMesh* GenerateQuad(simd::float4 colour = simd::make_float4(1,1,1,1),
                                float width = 1.0f,
                                float height = 1.0f,
                                MTL::Device* metalDevice = MTL::CreateSystemDefaultDevice(),
                                bool bCentralisedOrigin = true);
    
    // Generate a quad at a specific position
    static CMesh* GenerateQuadWithPosition(simd::float4 colour = simd::make_float4(1,1,1,1),
                                            const float x = 0.0f,
                                            const float y = 0.0f,
                                            const float width = 1.0f,
                                            const float height = 1.0f,
                                           MTL::Device* metalDevice = MTL::CreateSystemDefaultDevice());
};

#endif /* MeshBuilder_hpp */
