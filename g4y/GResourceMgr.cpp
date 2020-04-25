#include "GResourceMgr.h"
#include "GCommon.h"

GResourceMgr::GResourceMgr()
{
    // load cfg
}

GResourceMgr::~GResourceMgr()
{

}

bool GResourceMgr::LoadShader(std::string name, std::string vs, std::string fs, bool str)
{
    auto shader = std::make_shared<GShader>(vs, fs, str);
    if(shader->IsValid()){
        m_shaders[name] = shader;
        return true;
    }
    return false;
}

std::shared_ptr<GShader> GResourceMgr::Shader(std::string name)
{
    if(m_shaders.find(name) == m_shaders.end()) return nullptr;
    return m_shaders[name];
}

void GResourceMgr::ProcessNode(std::shared_ptr<GModelNodeInfo> r, aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        r->name = node->mName.C_Str();
        std::cout << "load node: " << node->mName.C_Str() << std::endl;
        r->SetEmpty(false);
        ProcessMesh(r, mesh, scene);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        auto c = std::make_shared<GModelNodeInfo>();
        r->children.push_back(c);
        ProcessNode(c, node->mChildren[i], scene);
    }
}
void GResourceMgr::ProcessMesh(std::shared_ptr<GModelNodeInfo> r,aiMesh *mesh, const aiScene *scene)
{
    auto gmesh = std::make_shared<GMesh>();

    auto& vertices = gmesh->m_vertices;
    auto& indices = gmesh->m_indices;
    std::vector<std::string> textures_name;

    std::cout << "load mesh: " << mesh->mName.C_Str() << std::endl;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        GVertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        // tangent
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.tangent = vector;
        // bitangent
        vector.x = mesh->mBitangents[i].x;
        vector.y = mesh->mBitangents[i].y;
        vector.z = mesh->mBitangents[i].z;
        vertex.bitangent = vector;
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "material.diffuse");
    textures_name.insert(textures_name.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "material.specular");
    textures_name.insert(textures_name.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    auto normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "material.normal");
    textures_name.insert(textures_name.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    auto heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "material.height");
    textures_name.insert(textures_name.end(), heightMaps.begin(), heightMaps.end());

    gmesh->SetupMesh();
    r->info.push_back(std::make_pair(gmesh, textures_name));
}

std::vector<std::string> GResourceMgr::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<std::string> textures_name;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        if(m_textures.find(str.C_Str()) == m_textures.end()){
            auto texture = std::make_shared<GTexture>();

            texture->LoadTextureFromFile(m_cur_model_path + '/' + str.C_Str());
            texture->m_type = typeName;
            texture->m_path = str.C_Str();
            m_textures[str.C_Str()] = texture;
            textures_name.push_back(str.C_Str());
        }else{
            textures_name.push_back(str.C_Str());
        }
    }
    return textures_name;
}

bool GResourceMgr::LoadModel(std::string const& filepath)
{
    if(m_models.find(filepath) != m_models.end()) return false;

    std::string abspath = m_resource_directory + filepath;

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(abspath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "[ERROR] ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    auto root = std::make_shared<GModelNodeInfo>();
    m_models[filepath] = root;
    m_cur_model_path = abspath.substr(0, abspath.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(root, scene->mRootNode, scene);
    return true;
}

void GResourceMgr::InstantiateModel(std::shared_ptr<GObj> p, std::shared_ptr<GModelNodeInfo> node)
{
    std::shared_ptr<GObj> obj = nullptr;
    if(node->Empty()){
        obj = std::make_shared<GObj>();
        obj->SetObjName(node->name);
        obj->AddDefaultComs();
        p->AddChild(obj);
    }else{
        for(int i = 0; i < node->info.size(); ++i){
            obj = std::make_shared<GObj>();
            obj->SetObjName(node->name);
            obj->AddDefaultComs();

            auto render = std::make_shared<GMeshRenderer>();
            obj->AddCom(render);

            p->AddChild(obj);

            render->m_mesh = node->info[i].first;
            for(int j = 0; j < node->info[i].second.size(); ++j){
                render->m_materials.emplace_back(GWorld::Instance()->GLView()->GetShader(), m_textures[node->info[i].second[j]]);
            }
        }
    }

    for(int i = 0; i < node->children.size(); ++i){
        InstantiateModel(obj, node->children[i]);
    }
}

std::shared_ptr<GObj> GResourceMgr::Instantiate(std::string name)
{
    if(m_models.find(name) == m_models.end()) return nullptr;
    auto info = m_models[name];

    auto obj = std::make_shared<GObj>();
    obj->AddDefaultComs();

    InstantiateModel(obj, info);
    return obj;
}

std::shared_ptr<GModelNodeInfo> FindChildNodeInfo(std::shared_ptr<GModelNodeInfo> info, std::string node_name)
{
    if(info == nullptr) return nullptr;

    if(info->name == node_name){
        return info;
    }
    for(int i = 0; i < info->children.size(); ++i){
        auto ret = FindChildNodeInfo(info->children[i], node_name);
        if(ret != nullptr) return ret;
    }
    return nullptr;
}

std::shared_ptr<GObj> GResourceMgr::CloneChildNode(std::string model_name, std::string node_name)
{
    if(m_models.find(model_name) == m_models.end()) return nullptr;
    auto root_info = m_models[model_name];
    auto node_info = FindChildNodeInfo(root_info, node_name);
    std::cout << "get " << node_info->name << std::endl;
    if(node_info == nullptr) return nullptr;
    // clone node obj
    auto obj = std::make_shared<GObj>();
    obj->SetObjName(node_info->name);
    obj->AddDefaultComs();

    auto render = std::make_shared<GMeshRenderer>();
    obj->AddCom(render);

    render->m_mesh = node_info->info[0].first;
    for(int j = 0; j < node_info->info[0].second.size(); ++j){
        render->m_materials.emplace_back(GWorld::Instance()->GLView()->GetShader(), m_textures[node_info->info[0].second[j]]);
    }

    return obj;
}



void GResourceMgr::Test()
{
    GResourceMgr mgr;
    mgr.SetResourceDir("/home/lkp/projs/g4y/asset/");

    mgr.LoadModel("nanosuit/nanosuit.obj");
}