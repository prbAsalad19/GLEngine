#include "ImageData.h"

unsigned char* loadImage(const char* filename, int& width, int& height, int& channels)
{
	return stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
}

void freeImage(unsigned char* data)
{
	stbi_image_free(data);
}