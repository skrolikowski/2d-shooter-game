#ifndef AUDIO_H
#define AUDIO_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "Util/SDLPtr.h"

struct Sound
{
    SDL_AudioSpec spec;
    SDL_WaveBufferPtr data;
    Uint32 length = 0;
};

class Audio
{
  public:
    static bool Init(); // open device, create stream pool
    static void Load(const std::string &name, const std::string &path);
    static void Play(const std::string &name);
    static void Shutdown(); // free sounds, destroy streams, close device

  private:
    inline static const int StreamSize = 4;
    inline static SDL_AudioDeviceID Device = 0;
    inline static std::unordered_map<std::string, Sound> Sounds;
    inline static std::vector<SDL_AudioStreamPtr> Streams; // the voice pool
};

#endif // AUDIO_H
