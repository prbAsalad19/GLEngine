```cpp
#pragma once

#include "opengl/OpenGLTexture.h"
#include "core/resourcemanager/ResourceHandle.h"

/**
 * @file Material.h
 * @brief Header file for the Material class which handles material properties and textures.
 */

namespace Core {

/**
 * @struct Material
 * @brief Represents a material used in rendering with various texture properties.
 */
struct Material {
    /**
     * @brief Mapping of texture names to their respective texture handles.
     */
    std::unordered_map<std::string, TextureHandle> textures;
};

} // namespace Core
```