#pragma once

#include <bits/unique_ptr.h>
#include <bits/shared_ptr.h>

namespace Quanta
{
    template<typename T>
    using Ptr = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;   

    template<typename T>
    using Weak = std::weak_ptr<T>;
}