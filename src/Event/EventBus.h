#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Event/Event.h"
#include "Event/Subscription.h"
#include "Util/Vector2.h"

class EventBus
{
  public:
    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;

    static EventBus &GetInstance()
    {
        static EventBus Instance;
        return Instance;
    }

    template <typename T, typename F> Subscription Subscribe(F &&fn);

    void Unsubscribe(Subscription::Token id);
    void Push(Event e);
    void Dispatch();

  private:
    EventBus() = default;
    ~EventBus() = default;

    struct Sub
    {
        Subscription::Token id;
        std::function<void(const Event &)> fn;
        bool isSubbed = true; // Why? -> Unsubs can happen during Update()
    };

    std::unordered_map<std::type_index, std::vector<Sub>> subs_;
    std::vector<Event> pending_, processing_;
    std::vector<Subscription::Token> removed_;
    Subscription::Token nextId_ = 1;
};

// --
/**
 * Dev-Notes:
 *
 * The compiler must see the template's body wherever it's used; put template methods in .h or .tpp
 */
template <typename T, typename F> Subscription EventBus::Subscribe(F &&fn)
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

    subs_[typeid(T)].push_back(
        Sub{.id = id, .fn = [f = std::forward<F>(fn)](const Event &e) { f(std::get<T>(e)); }});

    return Subscription{id};
}
