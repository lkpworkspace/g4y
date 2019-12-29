#ifndef __GSHADER_H__
#define __GSHADER_H__
#include <string>

class GShader
{
public:
    GShader(std::string vs_filepath, std::string fs_filepath);
    virtual ~GShader(){}
    
    void Use();
    unsigned int ID() { return m_id; }
    void SetUniform(const std::string &name, bool value) const;
    void SetUniform(const std::string &name, int value) const;
    void SetUniform(const std::string &name, float value) const;
private:

    void CheckCompileErrors(unsigned int shader, std::string type);

    unsigned int m_id;
};

#endif