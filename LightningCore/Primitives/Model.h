//
//  Model.h
//  LightningCore
//
//  Created by Kian Marvi on 7/26/26.
//

#ifndef Model_h
#define Model_h
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Mesh.h"

class Model
{
public:
    Model() = default;
    Model(const char* path);
    ~Model();
private:
    std::vector<Mesh> m_Mesh;
    std::string m_Directory;
    std::vector<Texture> textures_loaded; 
    
    void LoadModel(const char* path);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    void ProcessNode(aiNode *node, const aiScene *scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial *mat,
                                              aiTextureType type,
                                              const char* typeName);
};

#endif /* Model_h */
