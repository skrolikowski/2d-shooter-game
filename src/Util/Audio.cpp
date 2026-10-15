#include "Audio.h"

bool Audio::Init()
{
    Device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (Device == 0)
    {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return false;
    }

    Streams.reserve(StreamSize);
    for (int i = 0; i < StreamSize; ++i)
    {
        SDL_AudioStream *s = SDL_CreateAudioStream(nullptr, nullptr); // formats set later
        SDL_BindAudioStream(Device, s);
        Streams.emplace_back(s);
    }

    return true;
}

bool Audio::Load(const std::string &name, const std::string &path)
{
    // Audio..
    Sound s;
    Uint8 *buf = nullptr;
    if (!SDL_LoadWAV(path.c_str(), &s.spec, &buf, &s.length))
    {
        SDL_Log("SDL_LoadWAV failed: %s", SDL_GetError());
        return false;
    }
    s.data.reset(buf);
    Sounds.emplace(name, std::move(s));
    return true;
}

void Audio::Play(const std::string &name)
{
    const auto &it = Sounds.find(name);
    if (it != Sounds.end())
    {
        const Sound &sound = it->second;

        for (auto &stream : Streams)
        {
            if (SDL_GetAudioStreamQueued(stream.get()) < (int)sound.length)
            {
                SDL_SetAudioStreamFormat(stream.get(), &sound.spec, nullptr);
                SDL_PutAudioStreamData(
                    stream.get(), sound.data.get(), static_cast<int>(sound.length));
                return;
            }
        }
    }
}

void Audio::Shutdown()
{
    Sounds.clear();
    Streams.clear();

    SDL_CloseAudioDevice(Device);
}