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

#include <graphics/model.hpp>

#include <graphics/gUtils.hpp>

#include "../core/GL/gl.h"

namespace gfx
{
Model::Model()
    : m_mesh       {},
      m_diffuseMap {0}
{
}

Model::Model(const char* path, const char* texturePath, bool hasNormals, bool hasUVs)
    : m_mesh       {path, hasNormals, hasUVs},
      m_diffuseMap {0}
{
    m_diffuseMap = loadTexture(texturePath);
}

Model::~Model()
{
    glDeleteTextures(1, &m_diffuseMap);
}

void Model::draw(Shader& shader)
{
    shader.use();
    shader.setInt("material.texture_diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_diffuseMap);
    m_mesh.draw(shader);
}

} // namespace gfx
