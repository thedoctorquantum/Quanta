#pragma once

#include "../Math/Integer.h"

namespace Quanta
{
    template<typename T>
    class Span final
    {
    public:
        Span(T* pointer) : Span(pointer, 1) { };

        Span(T* pointer, Size size)
        {
            this->pointer = pointer;
            this->size = size;
        }

        T& operator[](Size index) const
        {
            return pointer[index];
        }

        T& operator*() const 
        {
            return *pointer;
        }

        T* operator->() const
        {
            return pointer;
        }

        T* GetPointer() const
        {
            return pointer;
        }

        Size GetLength() const
        {
            return length;
        }
    private:
        T* pointer = nullptr;
        Size length = 0;
    };
}