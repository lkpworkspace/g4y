#ifndef __GMODEL_H__
#define __GMODEL_H__
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GCom.h"
#include "GShader.h"
#include "GMesh.h"

class GCamera;
class GTransform;
class GModel : public GCom
{
public:
    GModel(std::string const &filepath, bool gamma = false);
    virtual ~GModel(){}

    bool                  gamma_correction;
    std::string           directory;
    std::vector<GTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<GMesh>    meshes;

    void Draw(std::shared_ptr<GShader> shader);

    virtual void Start() override;

    virtual void OnRender() override;

    virtual std::string ComName() override { return "GModel"; }
private:
    void LoadModel(std::string const &path);
    GMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<GTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void ProcessNode(aiNode *node, const aiScene *scene);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = true);

    std::weak_ptr<GShader>    m_shader;
    std::weak_ptr<GCamera>    m_camera;
    std::weak_ptr<GTransform> m_transform;
};

#endif

