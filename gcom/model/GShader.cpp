#include "GShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GShader::GShader(std::string vs_filepath, std::string fs_filepath)
{
    std::string vs_string;
    std::string fs_string;
    std::ifstream vs_file;
    std::ifstream fs_file;
    
    vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        vs_file.open(vs_filepath);
        fs_file.open(fs_filepath);

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

    const char *vs_str = vs_string.c_str();
    const char *fs_str = fs_string.c_str();
    
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_str, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs_str, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    CheckCompileErrors(m_id, "PROGRAM");
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void GShader::CheckCompileErrors(unsigned int shader, std::string type)
{
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
}


void GShader::Use() 
{ 
    glUseProgram(m_id); 
}

void GShader::SetUniform(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}

void GShader::SetUniform(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}

void GShader::SetUniform(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); 
}