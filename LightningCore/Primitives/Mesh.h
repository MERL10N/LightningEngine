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

struct MeshVertex
{
    float3 m_Position;
    float3 m_Normal;
    float2 m_TexCoords;
    float3 m_Tangent;
    float3 m_Bitangent;
};

struct MeshTexture
{
    std::string path;
    std::string type;
};

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(const std::vector<MeshVertex>    &vertices,
                  const std::vector<unsigned int>  &indices,
                  const std::vector<MeshTexture>   &textures);
    ~Mesh();
private:
    std::vector<MeshVertex>         m_Vertices;
    std::vector<unsigned int>       m_Indices;
    std::vector<MeshTexture>        m_Textures;
    
    void SetupMesh();
};

#endif /* Mesh_h */
