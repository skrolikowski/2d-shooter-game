#ifndef SDLPTR_H
#define SDLPTR_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

template <auto Fn>
struct Deleter { template <typename T> void operator()(T* p) const { Fn(p); } };

using WindowPtr = std::unique_ptr<SDL_Window,   Deleter<SDL_DestroyWindow>>;
using RendererPtr = std::unique_ptr<SDL_Renderer, Deleter<SDL_DestroyRenderer>>;
using TTF_TextPtr = std::unique_ptr<TTF_Text, Deleter<TTF_DestroyText>>;
using TTF_FontPtr = std::unique_ptr<TTF_Font, Deleter<TTF_CloseFont>>;
using TTF_TextEnginePtr = std::unique_ptr<TTF_TextEngine, Deleter<TTF_DestroyRendererTextEngine>>;
using SDL_AudioStreamPtr = std::unique_ptr<SDL_AudioStream, Deleter<SDL_DestroyAudioStream>>;
using SDL_TexturePtr = std::unique_ptr<SDL_Texture, Deleter<SDL_DestroyTexture>>;
using SDL_WaveBufferPtr = std::unique_ptr<Uint8, Deleter<SDL_free>>;

#endif // SDLPTR_H
