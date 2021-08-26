#pragma once

#include "../Math/Integer.h"

namespace Quanta
{
    template<typename T>
    class Span final
    {
    public:
        Span(T* pointer) : Span(pointer, 1) { };

        Span(T* pointer, USize size)
        {
            this->pointer = pointer;
            this->size = size;
        }

        T& operator[](USize index) const
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

        USize GetLength() const
        {
            return length;
        }
    private:
        T* pointer = nullptr;
        USize length = 0;
    };
}