#include "GMaterial.hpp"
#include "GCommon.h"

GMaterial::GMaterial(std::shared_ptr<GShader> s, std::shared_ptr<GTexture> t)
{
    shader = s;
    texture = t;
}