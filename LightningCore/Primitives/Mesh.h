//
//  Mesh.h
//  LightningCore
//
//  Created by Kian Marvi on 7/26/26.
//

#ifndef Mesh_h
#define Mesh_h

#include <hlsl++.h>
#include <vector>

using namespace hlslpp;

// Forward Declarations
struct aiNode;
struct aiScene;

struct Vertex
{
    float3 m_Position;
    float3 m_Normal;
    float2 m_TexCoords;
    float3 m_Tangent;
    float3 m_Bitangent;
};

struct Texture
{
    std::string m_Path;
    std::string m_Type;
};

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(const std::vector<Vertex>   &vertices,
                  const std::vector<unsigned int>  &indices,
                  const std::vector<Texture>       &textures);
    ~Mesh();
private:
    std::vector<Vertex>         m_Vertices;
    std::vector<unsigned int>   m_Indices;
    std::vector<Texture>        m_Textures;
    
    void SetupMesh();
};

#endif /* Mesh_h */
