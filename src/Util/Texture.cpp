#include "Texture.h"

SDL_Renderer* Texture::Renderer = nullptr;

Texture::Texture(const std::string& path)
{
    LoadFromFile(path);
}

void Texture::LoadFromFile(const std::string& path)
{
    path_ = path;
    texture_ = IMG_LoadTexture(Texture::Renderer, path.c_str());
    if (!texture_)
    {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }

    SDL_GetTextureSize(texture_, &width_, &height_);
}

void Texture::UnloadFile()
{
    if (texture_)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
}

void Texture::Draw(SDL_Renderer* renderer, float x, float y, float scaleX, float scaleY)
{
    // Note: use full image
    // const SDL_FRect srcRect
    // {
    //     .w = width_,
    //     .h = height_,
    // };

    const SDL_FRect dstRect
    {
        .x = x,
        .y = y,
        .w = width_ * scaleX,
        .h = height_ * scaleY,
    };

    SDL_RenderTexture(renderer, texture_, nullptr, &dstRect);
}