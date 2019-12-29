#ifndef __GMODEL_H__
#define __GMODEL_H__
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GShader.h"
#include "GMesh.h"

class GModel
{
public:
    GModel(){}
    virtual ~GModel(){}

    bool                  gamma_correction;
    std::string           directory;
    std::vector<GTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<GMesh>    meshes;

    GModel(std::string const &filepath, bool gamma = false);

    void Draw(GShader shader);

private:
    void LoadModel(std::string const &path);
    GMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<GTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void ProcessNode(aiNode *node, const aiScene *scene);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = true);
};

#endif

