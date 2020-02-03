#include "GResourceMgr.h"
#include "GShader.h"

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