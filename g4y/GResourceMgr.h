#ifndef __GRESOURCEMGR_H__
#define __GRESOURCEMGR_H__
#include <string>
#include <memory>
#include <unordered_map>

class GShader;
class GResourceMgr
{
public:
    /* shader管理 */
    bool LoadShader(std::string name, std::string vs, std::string fs, bool str = true);
    std::shared_ptr<GShader> Shader(std::string);

    /* 模型管理 */
    /* texture管理 */
    /* 声音管理 */
    /* TODO... */

private:
    std::unordered_map<std::string, std::shared_ptr<GShader>> m_shaders;
};

#endif