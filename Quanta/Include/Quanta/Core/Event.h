#pragma once

#include <memory>
#include <vector>
#include <functional>

namespace Quanta
{
    template<typename... Args> 
    class Event final
    {
    public:
        typedef std::function<void(Args...)> Handler;

        inline void operator+=(const Handler& handler)
        {
            listeners.push_back(handler);
        }

        inline void operator()(Args... args) const
        {
            for(size_t i = 0; i < listeners.size(); i++)
            {
                const Handler& handler = listeners[i];

                handler(args...);
            }
        }
    private:
        std::vector<Handler> listeners; 
    };
}