#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <memory>
#include <string>

class Texture
{
public:
    explicit Texture(const std::string& path);

    void LoadFromFile(const std::string& path);
    void UnloadFile();

    void Draw(SDL_Renderer* renderer, float x, float y, float scaleX = 1.f, float scaleY = 1.f);

    float GetWidth() const { return width_; }
    float GetHeight() const { return height_; }

    static void SetDefaultRenderer(SDL_Renderer* defaultRenderer)
        { Renderer = defaultRenderer; }

private:
    std::string path_;
    float width_;
    float height_;

    SDL_Texture* texture_ = nullptr;
    static SDL_Renderer* Renderer;
};

#endif // TEXTURE_H