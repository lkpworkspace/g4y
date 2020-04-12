#include "GTexture.hpp"
#include "GResourceMgr.h"
#include <iostream>
#include <GL/glew.h>
#include "GObj.h"
#include "GScene.h"
#include "GCamera.h"
#include "GTransform.h"
#include "GOpenGLView.h"
#include "GWorld.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GTexture::GTexture() :
    m_vaild(false)
{

}


bool GTexture::LoadTextureFromFile(std::string filepath)
{
    m_path = filepath;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "[ERROR] Texture failed to load at path: " << m_path << std::endl;
        stbi_image_free(data);
        return false;
    }

    m_id = texture_id;
    m_width = width;
    m_height = height;
    return true;
}