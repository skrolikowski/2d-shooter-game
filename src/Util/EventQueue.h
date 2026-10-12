#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <variant>

#include "World/Entity.h"
#include "Util/Vector2.tpp"

struct InvaderKilled { Vector2<float> pos; InvaderType type; };
struct PlayerHit {};
struct WaveCleared {};
struct ShotFired { Vector2<float> pos; bool isPlayerFire = false; };
// --
using Event = std::variant<InvaderKilled, PlayerHit, WaveCleared, ShotFired>;

class EventQueue
{
public:
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    static EventQueue& GetInstance()
    {
        static EventQueue Instance;
        return Instance;
    }

    void Push(Event e)
    {
        pending_.push_back(e);
    }

    template <typename F>
    void Dispatch(F&& handle)
    {
        processing_.swap(pending_);

        for (const Event& e : processing_)
            handle(e);
        
        processing_.clear();
    }

private:
    EventQueue() = default;
    ~EventQueue() = default;

    std::vector<Event> pending_, processing_;

};

// Helper that turns several lambdas into one visitor
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };