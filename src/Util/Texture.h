#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

#include <optional>
#include <memory>
#include <string>
#include <unordered_map>

#include "SDLPtr.h"

struct Frame
{
    std::string name;
    SDL_FRect rect;
};

class Texture
{
public:
    static void Draw(const Frame* frame,  float x, float y, float scaleX = 1.f, float scaleY = 1.f);
    static const Frame* GetFrame(std::string filename);
    static bool LoadSpritesheet(const std::string& path);
    static void SetDefaultRenderer(SDL_Renderer* defaultRenderer)
        { Renderer = defaultRenderer; }
    static void UnloadFile();
    static bool HasFrame(std::string filename);

    inline static SDL_Renderer* Renderer = nullptr;

private:
    inline static std::unordered_map<std::string, Frame> Frames;
    inline static SDL_TexturePtr texture;
};

#endif // TEXTURE_H