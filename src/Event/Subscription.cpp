#include "Subscription.h"
#include "EventBus.h"

Subscription::~Subscription()
{
    if (id_)
        EventBus::GetInstance().Unsubscribe(id_);
}
