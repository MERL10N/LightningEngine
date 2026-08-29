//
//  Mesh.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/26/26.
//

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <print>

Mesh::Mesh(const std::vector<Vertex>  &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Texture>      &textures)
: m_Vertices(vertices),
  m_Indices(indices),
  m_Textures(textures)
{
}

