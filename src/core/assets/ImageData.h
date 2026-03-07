#pragma once
#include "core/CoreConfig.h"

struct Image
{
    unsigned char* data = nullptr;
    int      width = 0;
    int      height = 0;
    int      channels = 0;   // canali dell'immagine originale
    bool     valid = false;

    // Carica con stb_image — sempre forzato a 4 canali (RGBA)
    static Image load(const std::string& filepath);

    // IMPORTANTE: libera la memoria stb
    void free();
};