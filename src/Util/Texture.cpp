#include "Texture.h"

#include <format>
#include <fstream>
#include <nlohmann/json.hpp>

bool Texture::LoadSpritesheet(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        SDL_Log("Could not open: %s", path.c_str());
        return false;
    }

    std::filesystem::path imagePath;
    // float imageWidth, imageHeight;
    try
    {
        // --
        json data = json::parse(file); 
        const json& frameData = data.at("frames");
        for (const auto& data : frameData)
        {
            const std::string& name = data.at("filename").get<std::string>();
            Frame frame{
                .name = name,
                .rect = SDL_FRect{
                    .x = data.at("frame").at("x").get<float>(),
                    .y = data.at("frame").at("y").get<float>(),
                    .w = data.at("frame").at("w").get<float>(),
                    .h = data.at("frame").at("h").get<float>(),
                }
            };
            Frames.emplace(name, frame);
        }

        // --
        const std::string& imageFilename = data.at("meta").at("image").get<std::string>();
        imagePath = std::filesystem::path(path).parent_path() / imageFilename;
    }
    catch (const json::exception& e)
    {
        SDL_Log("Error parsing spritesheet: %s", e.what());
        return false;
    }

    // --
    texture.reset(IMG_LoadTexture(Renderer, imagePath.c_str()));
    if (!texture)
    {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Texture::UnloadFile()
{
    Frames.clear();
    texture.reset();
}

bool Texture::HasFrame(std::string filename)
{
    const auto& it = Frames.find(filename);
    return (it != Frames.end());
}

const Frame* Texture::GetFrame(std::string filename)
{
    const auto& it = Frames.find(filename);
    if (it == Frames.end())
    {
        SDL_Log("Cannot find frame: %s", filename.c_str());
        return nullptr;
    }
    return &it->second;
}

void Texture::Draw(const Frame* frame, float x, float y, float scaleX, float scaleY)
{
    const SDL_FRect& srcRect = frame->rect;
    SDL_FRect dstRect{ x, y, srcRect.w * scaleX, srcRect.h * scaleY };
    SDL_RenderTexture(Renderer, texture.get(), &srcRect, &dstRect);
}