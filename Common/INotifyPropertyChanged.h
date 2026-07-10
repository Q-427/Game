#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

class INotifyPropertyChanged
{
public:
    using Handler = std::function<void(const std::string&)>;
    using SubscriptionId = std::size_t;

    virtual ~INotifyPropertyChanged() = default;

    SubscriptionId subscribe(Handler handler)
    {
        const SubscriptionId id = nextSubscriptionId++;
        subscriptions.push_back({id, std::move(handler)});
        return id;
    }

    void unsubscribe(SubscriptionId id)
    {
        for (auto it = subscriptions.begin(); it != subscriptions.end(); ++it)
        {
            if (it->id == id)
            {
                subscriptions.erase(it);
                return;
            }
        }
    }

protected:
    void notifyPropertyChanged(const std::string& propertyName) const
    {
        const auto currentSubscriptions = subscriptions;

        for (const auto& subscription : currentSubscriptions)
        {
            if (subscription.handler)
                subscription.handler(propertyName);
        }
    }

private:
    struct Subscription
    {
        SubscriptionId id;
        Handler handler;
    };

    std::vector<Subscription> subscriptions;
    SubscriptionId nextSubscriptionId{1};

};
