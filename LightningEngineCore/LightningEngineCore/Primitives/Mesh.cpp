//
//  Mesh.c
//  LightningEngineCore
//
//  Created by Kian Marvi on 9/13/24.
//

#include "Mesh.h"
#include <Metal/Metal.hpp>

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices, MTL::Device *metalDevice)
: vertices(vertices),
  indices(indices),
  metalDevice(metalDevice)
{
    vertexBuffer = metalDevice->newBuffer(vertices.data(), sizeof(Vertex) * vertices.size(), MTL::ResourceStorageModeShared); // Allocate the vertex buffer given the size of the vertices
    
    indexBuffer = metalDevice->newBuffer(indices.data(), sizeof(uint32_t) * indices.size(), MTL::ResourceStorageModeShared); // Allocate the index buffer given the size of the indices
    
    assert(vertexBuffer != nullptr || indexBuffer != nullptr);
    
}

Mesh::~Mesh()
{
    if (vertexBuffer)
        vertexBuffer->release();
    
    if (indexBuffer)
        indexBuffer->release();
}



