#include "TextureManager.h"

std::shared_ptr<Texture> TextureManager::Get(const std::string& path)
{
    auto it = Textures.find(path);
    if (it == Textures.end())
        it = Textures.emplace(path, std::make_shared<Texture>(path)).first;
    return it->second;
}

void TextureManager::Destroy()
{
    for (const auto& [key, value] : Textures)
        value->UnloadFile();
}