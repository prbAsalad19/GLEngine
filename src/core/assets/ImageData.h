#pragma once
#include "stb_image.h"

unsigned char* loadImage(const char* filename, int& width, int& height, int& channels);
void freeImage(unsigned char* data);