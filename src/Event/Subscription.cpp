#include "Subscription.h"
#include "EventBus.h"

Subscription::~Subscription()
{
    if (id_ > 0)
        EventBus::GetInstance().Unsubscribe(id_);
}
