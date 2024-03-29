/********************************************************************************
 *                                                                              *
 * Curly Engine                                                                 *
 * Copyright (c) 2021-2024 Adrian Bedregal                                      *
 *                                                                              *
 * This software is provided 'as-is', without any express or implied            *
 * warranty. In no event will the authors be held liable for any damages        *
 * arising from the use of this software.                                       *
 *                                                                              *
 * Permission is granted to anyone to use this software for any purpose,        *
 * including commercial applications, and to alter it and redistribute it       *
 * freely, subject to the following restrictions:                               *
 *                                                                              *
 * 1. The origin of this software must not be misrepresented; you must not      *
 *    claim that you wrote the original software. If you use this software      *
 *    in a product, an acknowledgment in the product documentation would be     *
 *    appreciated but is not required.                                          *
 * 2. Altered source versions must be plainly marked as such, and must not be   *
 *    misrepresented as being the original software.                            *
 * 3. This notice may not be removed or altered from any source distribution.   *
 *                                                                              *
 ********************************************************************************/

#include <graphics/shader.hpp>

#include <exception/graphics/graphicsException.hpp>

#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../core/GL/gl.h"

namespace gfx
{
Shader::Shader(const std::string& t_name)
{
    std::string vsSrc {loadShaderFromFile(ShaderType::VERTEX_SHADER, "shaders/" + t_name)};
    std::string fsSrc {loadShaderFromFile(ShaderType::FRAGMENT_SHADER, "shaders/" + t_name)};

#if defined(C__CURLY_FORCE_GLX_CTX_VERSION)
    constexpr int glxCtxMajorVersion{ C__CURLY_GLX_CTX_VERSION_MAJOR };
    constexpr int glxCtxMinorVersion{ C__CURLY_GLX_CTX_VERSION_MINOR };
    const auto replaceGlxContextVersion = [](std::string& shaderSrc) -> void {
        const size_t versionDeclIdx = shaderSrc.find("#version");
        size_t versionDefStartIdx = versionDeclIdx + 9;
        while ((versionDefStartIdx < shaderSrc.size()) && !std::isdigit(shaderSrc[versionDefStartIdx]))
        {
            ++versionDefStartIdx;
        }
        size_t versionDefEndIdx = versionDefStartIdx + 1;
        while ((versionDefEndIdx < shaderSrc.size()) && std::isdigit(shaderSrc[versionDefEndIdx]))
        {
            ++versionDefEndIdx;
        }
        if ((versionDefStartIdx >= shaderSrc.size()) || (versionDefEndIdx >= shaderSrc.size()))
        {
            throw GraphicsException();
        }
        std::string glxCtxVersionStr = std::to_string(glxCtxMajorVersion) + std::to_string(glxCtxMinorVersion) + std::string("0");
        shaderSrc.replace(versionDefStartIdx, versionDefEndIdx - versionDefStartIdx, glxCtxVersionStr);
    };

    try
    {
        replaceGlxContextVersion(vsSrc);
        replaceGlxContextVersion(fsSrc);
    }
    catch (const GenericException& e)
    {
        std::cerr << "An Exception has occurred: " << e.what() << std::endl;
    }
#endif

    cfg::uint32 vertexShader {createShader(ShaderType::VERTEX_SHADER, vsSrc)};
    cfg::uint32 fragmentShader {createShader(ShaderType::FRAGMENT_SHADER, fsSrc)};

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    checkErrors(m_program, true);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader(const std::string& vsSrc, const std::string& fsSrc)
{
    cfg::uint32 vertexShader {createShader(ShaderType::VERTEX_SHADER, vsSrc)};
    cfg::uint32 fragmentShader {createShader(ShaderType::FRAGMENT_SHADER, fsSrc)};

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    checkErrors(m_program, true);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::use() const
{
    glUseProgram(m_program);
}

void Shader::setBool(const std::string& name, bool v0)
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)v0);
}

void Shader::setInt(const std::string& name, int v0)
{
    glUniform1i(glGetUniformLocation(m_program, name.c_str()), v0);
}

void Shader::setFloat(const std::string& name, float v0)
{
    glUniform1f(glGetUniformLocation(m_program, name.c_str()), v0);
}

void Shader::setVec2(const std::string& name, float v0, float v1)
{
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), v0, v1);
}

void Shader::setVec3(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), v0, v1, v2);
}

void Shader::setVec4(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), v0, v1, v2, v3);
}

void Shader::setVec2(const std::string& name, const math::Vec2& v)
{
    glUniform2f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y);
}

void Shader::setVec3(const std::string& name, const math::Vec3& v)
{
    glUniform3f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z);
}

void Shader::setVec4(const std::string& name, const math::Vec4& v)
{
    glUniform4f(glGetUniformLocation(m_program, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat3(const std::string& name, const glm::mat3& m0)
{
    glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(m0));
}

void Shader::setMat4(const std::string& name, const glm::mat4& m0)
{
    glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(m0));
}

cfg::uint32 Shader::createShader(const ShaderType type, const std::string& src)
{
    cfg::uint32 shader = glCreateShader(type);
    const char* shaderSrc {src.c_str()};

    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);

    checkErrors(shader, false);

    return shader;
}

std::string Shader::loadShaderFromFile(const ShaderType type, const std::string& path)
{
    std::ifstream shaderFile;
    std::stringstream shaderStrStream;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string extension {(type == ShaderType::VERTEX_SHADER) ? ".vs" : ".fs"};
    try
    {
        shaderFile.open(path + extension);
        shaderStrStream << shaderFile.rdbuf();
        shaderFile.close();
    }
    catch(const std::ifstream::failure& e)
    {
        std::cerr << "Error while loading shader:\n" << e.what() << std::endl;
    }
    return shaderStrStream.str();
}

void Shader::checkErrors(const cfg::uint32 target, const bool isProgram)
{
    cfg::int32 success;
    char infolog[512];

    if(isProgram)
    {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(target, 512, nullptr, infolog);
            std::cerr << "ERROR in Shader: Link Failed\n" << infolog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(target, 512, nullptr, infolog);
            std::cerr << "ERROR in Shader: Compilation Failed\n" << infolog << std::endl;
        }
    }
}

} // namespace gfx
