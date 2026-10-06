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
    TextureManager() = default;

    std::shared_ptr<Texture> Get(const std::string& path);
    void Destroy();

private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> loadedTextures_;

};

#endif // TEXTUREMANAGER_H
