#include "ImageData.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageData ImageData::load(const std::string& filepath)
{
    ImageData img;
    img.pixels = stbi_load(filepath.c_str(),
        &img.width, &img.height, &img.channels,
        STBI_rgb_alpha);
    img.valid = (img.pixels != nullptr);
    return img;
}

void ImageData::free()
{
    if (pixels) { stbi_image_free(pixels); pixels = nullptr; }
    valid = false;
}