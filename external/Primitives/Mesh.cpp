//
//  Mesh.cpp
//  LightningEngine
//  Created by Kian Marvi on 2/2/24.

#include "Mesh.h"

/**
 @brief Constructor
 */
CMesh::CMesh(MTL::Device* device)
: mode(DRAW_TRIANGLES)
{
    this->device = device;
    // vertexBuffer = device->newBuffer(vertices.data(), vertexBufferSize, MTL::ResourceStorageModeShared);
//    indexBuffer = device->newBuffer(vertexIndices.data(), indexBufferSize, MTL::ResourceStorageModeShared);
}

CMesh::~CMesh(void)
{
    vertexBuffer->release();
    indexBuffer->release();
}

CMesh::Render()
{
}




