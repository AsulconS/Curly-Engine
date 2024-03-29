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

#pragma once

#include <external/glm/glm.hpp>

#include <core/config.hpp>
#include <core/common.hpp>

#include <system/dstr/vector.hpp>

#include <graphics/shader.hpp>

namespace gfx
{
/**
 * @brief Load an OBJ file from a path and load the vertexData and indices into the parameters
 * 
 * @param path 
 * @param vertexData 
 * @param indices 
 * @param hasNormals 
 * @param hasUVs 
 * @return bool 
 */
CURLY_API bool loadObj(const char* path, sys::Vector<float>& vertexData, sys::Vector<cfg::uint32>& indices, bool hasNormals = true, bool hasUVs = true);

/**
 * @brief Load a texture from a path and return the texture object created by OpenGL
 * 
 * @param path 
 * @return cfg::uint32 
 */
CURLY_API cfg::uint32 loadTexture(const char* path);

/**
 * @brief Setup the Default Lights for a shader from some view position
 * 
 * @param shader 
 * @param viewPos 
 */
CURLY_API void setupDefaultLights(Shader& shader, const glm::vec3& viewPos = {2.0f, 4.0f, 2.0f});

} // namespace gfx
