#include "TextureManager.h"

std::shared_ptr<Texture> TextureManager::Get(const std::string& path)
{
    auto it = loadedTextures_.find(path);
    if (it == loadedTextures_.end())
        loadedTextures_[path] = std::make_shared<Texture>(path);

    return loadedTextures_[path];
}

void TextureManager::Destroy()
{
    for (const auto& [key, value] : loadedTextures_)
        static_cast<std::shared_ptr<Texture>>(value)->UnloadFile();
}