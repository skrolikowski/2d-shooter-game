#pragma once

#include <functional>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <utility>

#include "Event/Event.h"
#include "Event/Subscription.h"
#include "Util/Vector2.h"
#include "World/Entity.h"

class EventBus
{
public:
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    static EventBus& GetInstance()
    {
        static EventBus Instance;
        return Instance;
    }

    template <typename T, typename F>
    Subscription Subscribe(F&& fn);

    void Unsubscribe(Subscription::Token id);
    void Push(Event e);
    void Dispatch();

private:
    EventBus() = default;
    ~EventBus() = default;

    struct Sub { Subscription::Token id; std::function<void(const Event&)> fn; bool isSubbed = true; };

    std::unordered_map<std::type_index, std::vector<Sub>> subs_;
    std::vector<Event> pending_, processing_;
    std::vector<Subscription::Token> removed_;
    Subscription::Token nextId_ = 1;
};

// --
template <typename T, typename F>
Subscription EventBus::Subscribe(F&& fn)
{
    /**
     * Dev Notes:
     * 
     * std::is_constructible_v<Event, T> asks: "can I make an Event from a T?"
     * Event is your std::variant<InvaderKilled, PlayerHit, WaveCleared, ShotFired>,
     * so in practice it asks whether T is one of those four types.
     */
    static_assert(std::is_constructible_v<Event, T>, "T is not an Event type");

    Subscription::Token id = nextId_++;

    subs_[typeid(T)].push_back(Sub{
        .id = id,
        .fn = [f = std::forward<F>(fn)](const Event& e) { f(std::get<T>(e)); }
    });

    return Subscription{ id };
}
