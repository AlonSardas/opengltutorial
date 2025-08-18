#pragma once

#include "Mesh.h"
#include "Shader.h"
// #include "badmesh.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

unsigned int loadTextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
  public:
    std::vector<TextureData> loadedTextures;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    Model(const std::string &path, bool gamma = false);
  ~Model();

    void draw(Shader &shader);

  private:
    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<TextureData> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
};
