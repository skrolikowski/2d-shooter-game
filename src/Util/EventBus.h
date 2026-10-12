#pragma once

#include <functional>
#include <typeindex>
#include <vector>
#include <variant>
#include <unordered_map>
#include <utility>

#include "World/Entity.h"
#include "Util/Vector2.h"

struct InvaderKilled { Vector2<float> pos; InvaderType type; };
struct PlayerHit {};
struct WaveCleared {};
struct ShotFired { Vector2<float> pos; bool isPlayerFire = false; };
// --
using Event = std::variant<InvaderKilled, PlayerHit, WaveCleared, ShotFired>;


class Subscription;

class EventBus
{
public:
    using Token = std::size_t;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    static EventBus& GetInstance()
    {
        static EventBus Instance;
        return Instance;
    }

    template <typename T, typename F>
    Subscription Subscribe(F&& fn);

    void Unsubscribe(Token id);
    void Push(Event e);
    void Dispatch();

private:
    EventBus() = default;
    ~EventBus() = default;

    struct Sub { Token id; std::function<void(const Event&)> fn; bool isSubbed = true; };

    std::unordered_map<std::type_index, std::vector<Sub>> subs_;
    std::vector<Event> pending_, processing_;
    std::vector<Token> removed_;
    Token nextId_ = 1;
};

// --
/**
 * Dev Note:
 * It's an attribute, a note to the compiler written in [[...]].
 * It means "the caller shouldn't ignore this function's return value."
 * If someone calls the function and throws the result away, the compiler gives a warning.
 * 
 * 1) Subscribe returns a temporary Subscription.
 * 2) Nobody stores it, so it's destroyed at the end of that line.
 * 3) Its destructor calls Unsubscribe.
 * 4) The handler is removed right after being added and never fires. There's no crash and no error. It just doesn't work.
 * 
 * [[nodiscard]] catches exactly that.
 */
class [[nodiscard]] Subscription
{
public:
    explicit Subscription(EventBus::Token id) : id_(id) {}
    Subscription(Subscription&& o) noexcept : id_(std::exchange(o.id_, 0)) {}
    ~Subscription() { if (id_) EventBus::GetInstance().Unsubscribe(id_); }
    Subscription& operator=(Subscription&&) = delete;
    Subscription(const Subscription&) = delete;

private:
    EventBus::Token id_;

};

// --
template <typename T, typename F>
Subscription EventBus::Subscribe(F&& fn)
{
    // checks whether a type T can be constructed with a given set of argument types
    static_assert(std::is_constructible_v<Event, T>, "T is not an Event type");

    Token id = nextId_++;

    subs_[typeid(T)].push_back(Sub{
        .id = id,
        .fn = [f = std::forward<F>(fn)](const Event& e) { f(std::get<T>(e)); }
    });

    return Subscription{ id };
}
