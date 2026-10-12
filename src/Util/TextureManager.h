#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL3/SDL.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "Texture.h"

class TextureManager
{
public:
    inline static std::unordered_map<std::string, std::shared_ptr<Texture>> Textures;
    
    static std::shared_ptr<Texture> Get(const std::string& path);
    static void Destroy();

};

#endif // TEXTUREMANAGER_H
