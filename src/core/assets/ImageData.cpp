#include "ImageData.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image Image::load(const std::string& filepath)
{
    Image img;
    img.data = stbi_load(filepath.c_str(),
        &img.width, &img.height, &img.channels,
        STBI_rgb_alpha);
    img.valid = (img.data != nullptr);
    return img;
}

void Image::free()
{
    if (data) { stbi_image_free(data); data = nullptr; }
    valid = false;
}