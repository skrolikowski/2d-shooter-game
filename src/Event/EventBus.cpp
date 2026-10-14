#include "EventBus.h"

void EventBus::Unsubscribe(Subscription::Token id)
{
    for (auto &[type, list] : subs_)
        for (auto &s : list)
            if (s.id == id)
                s.isSubbed = false;

    removed_.push_back(id);
}

void EventBus::Push(Event e)
{
    pending_.push_back(e);
}

void EventBus::Dispatch()
{
    processing_.swap(pending_);

    // Process Events..
    for (const Event &e : processing_)
    {
        auto type = std::visit([](const auto &v) { return std::type_index(typeid(v)); }, e);

        if (auto it = subs_.find(type); it != subs_.end())
        {
            auto &list = it->second;
            for (size_t i = 0, n = list.size(); i < n; ++i)
                if (list[i].isSubbed)
                    list[i].fn(e);
        }
    }
    processing_.clear();

    // Remove unsubscribed..
    for (Subscription::Token id : removed_)
        for (auto &[type, list] : subs_)
            std::erase_if(list, [id](const Sub &s) { return s.id == id; });
    removed_.clear();
}