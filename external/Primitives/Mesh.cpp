//  Mesh.cpp
//  LightningEngine
//  Created by Kian Marvi on 2/2/24.

#include "Mesh.h"

/**
 @brief Constructor
 */
CMesh::CMesh(MTL::Device* device)
: mode(DRAW_TRIANGLES),
  device(device)
{
    vertexBuffer = device->newBuffer(v.data(), sizeof(Vertex) * v.size(), MTL::ResourceStorageModeShared);
    indexSize = indices.size();
    indexBuffer = device->newBuffer(v.data(), sizeof(uint32_t) * indexSize, MTL::ResourceStorageModeShared);
     
}

CMesh::~CMesh(void)
{
    vertexBuffer->release();
    indexBuffer->release();
}

void CMesh::Render(MTL::RenderCommandEncoder* renderCommandEncoder)
{
    switch (mode)
    {
        case DRAW_LINES:
            renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeLine, NS::UInteger(0), NS::UInteger(0));
            break;
        case DRAW_TRIANGLE_STRIP:
            renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangleStrip, NS::UInteger(0), NS::UInteger(6));
            break;
        default:
            renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(6));
            break;
            
    }
}




