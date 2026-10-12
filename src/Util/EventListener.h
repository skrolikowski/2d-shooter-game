#pragma once
#include <vector>
#include <utility>

#include "Util/EventBus.h"

/**
 * Usage:
 * ------
 * 1) Inherit EventListener 
 * class App : public EventListener { ... };
 * 
 * 2) Example:
 * Listen<PlayerHit>([this](const PlayerHit&) { handlePlayerHit(); });
 * 
 */

class EventListener
{
protected:
    template <typename T, typename F>
    void Listen(F&& fn)
    {
        subscriptions_.push_back(
            EventBus::GetInstance().Subscribe<T>(std::forward<F>(fn)));
    }

private:
    std::vector<Subscription> subscriptions_;

};


