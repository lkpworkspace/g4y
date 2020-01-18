#include "GShader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#ifdef USE_GUI
#include <GL/glew.h>
#endif
GShader::GShader(const std::string &vs, const std::string &fs, bool filepath)
{
    if(filepath){
        std::string vs_string;
        std::string fs_string;
        std::ifstream vs_file;
        std::ifstream fs_file;
        
        vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            vs_file.open(vs);
            fs_file.open(fs);

            std::stringstream vs_stream, fs_stream;
            vs_stream << vs_file.rdbuf();
            fs_stream << fs_file.rdbuf();

            vs_file.close();
            fs_file.close();

            vs_string   = vs_stream.str();
            fs_string = fs_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "[ERROR] SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        CompileShader(vs_string.c_str(), fs_string.c_str());
    }else{
        CompileShader(vs.c_str(), fs.c_str());
    }
}

GShader::~GShader()
{}

void GShader::CompileShader(const char* vs_code, const char* fs_code)
{    
#ifdef USE_GUI
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_code, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs_code, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    CheckCompileErrors(m_id, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
#endif
}

void GShader::CheckCompileErrors(unsigned int shader, std::string type)
{
#ifdef USE_GUI
    int success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cout << "[ERROR] SHADER_COMPILATION_ERROR of type: " 
                    << type << "\n" << info_log 
                    << "\n" 
                    << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cout << "[ERROR] PROGRAM_LINKING_ERROR of type: " 
                    << type << "\n" << info_log << "\n" 
                    << std::endl;
        }
    }
#endif
}


void GShader::Use() 
{ 
#ifdef USE_GUI
    glUseProgram(m_id);
#endif
}

void GShader::SetUniform(const std::string &name, bool value) const
{
#ifdef USE_GUI
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
#endif
}

void GShader::SetUniform(const std::string &name, int value) const
{
#ifdef USE_GUI
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
#endif
}

void GShader::SetUniform(const std::string &name, float value) const
{
#ifdef USE_GUI
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
#endif
}

void GShader::SetUniform(const std::string &name, glm::vec2& value) const 
{
#ifdef USE_GUI
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
#endif
}
void GShader::SetUniform(const std::string &name, float x, float y) const
{
#ifdef USE_GUI
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
#endif
}
void GShader::SetUniform(const std::string &name, glm::vec3& value) const 
{
#ifdef USE_GUI
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
#endif
}
void GShader::SetUniform(const std::string &name, float x, float y, float z) const 
{
#ifdef USE_GUI
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
#endif
}
void GShader::SetUniform(const std::string &name, glm::vec4& value) const 
{
#ifdef USE_GUI
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
#endif
}
void GShader::SetUniform(const std::string &name, float x, float y, float z, float w) const
{
#ifdef USE_GUI
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
#endif
}

void GShader::SetUniform(const std::string &name, glm::mat2& mat) const 
{
#ifdef USE_GUI
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
#endif
}
void GShader::SetUniform(const std::string &name, glm::mat3& mat) const 
{
#ifdef USE_GUI
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
#endif
}
void GShader::SetUniform(const std::string &name, glm::mat4& mat) const 
{
#ifdef USE_GUI
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
#endif
}