#pragma once

#include <functional>
#include <map>
#include <cstdint>

template <typename... Args>
class EventSource
{
public:
    using Cb = std::function<void(Args...)>;

    uint64_t Subscribe(const Cb &callback)
    {
        subs_.insert({next_id_, callback});
        return next_id_++;
    }

    void Unsubscribe(uint64_t id)
    {
        subs_.erase(id);
    }

    void Trigger(Args... args)
    {
        for (const auto &[_id, cb] : subs_)
        {
            cb(args...);
        }
    }

private:
    uint64_t next_id_ = 0;
    std::map<uint64_t, Cb> subs_;
};
