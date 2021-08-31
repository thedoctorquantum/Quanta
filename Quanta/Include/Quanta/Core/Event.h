#pragma once

#include <vector>
#include <functional>

namespace Quanta
{
    template<typename... Args> 
    class Event final
    {
    public:
        using Handler = std::function<void(Args...)>;

        void operator+=(const Handler& handler)
        {
            listeners.push_back(handler);
        }

        void operator()(Args... args) const
        {
            for (const Handler& handler : listeners)
            {
                handler(args...);
            }
        }
    private:
        std::vector<Handler> listeners; 
    };
}