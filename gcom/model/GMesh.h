#ifndef __GMESH_H__
#define __GMESH_H__
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GShader.h"

struct GVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct GTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class GMesh
{
    
public:
    GMesh(std::vector<GVertex> vertices, std::vector<unsigned int> indices, std::vector<GTexture> textures);
    virtual ~GMesh(){}

    void Draw(std::shared_ptr<GShader> shader);

    std::vector<unsigned int> indices;
    std::vector<GVertex>      vertices;
    std::vector<GTexture>     textures;
    unsigned int VAO;
private:
    unsigned int VBO, EBO;

    void SetupMesh();

};

#endif

