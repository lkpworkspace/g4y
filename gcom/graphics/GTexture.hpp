#ifndef __GTEXTURE_HPP__
#define __GTEXTURE_HPP__
#include <string>

class GTexture
{
public:
    GTexture();

    bool LoadTextureFromFile(std::string);

    bool              m_vaild;
    unsigned int      m_id;
    int               m_width;
    int               m_height;
    std::string       m_type;
    std::string       m_path;
};

#endif