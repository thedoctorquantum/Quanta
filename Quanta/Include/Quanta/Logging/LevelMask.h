#pragma once

#include "Level.h"

namespace Quanta::Log
{
    enum class LevelMask : U8
    {
        None = 0b00000000,
        All = 0b11111111,

        Information = static_cast<U8>(Level::Information),
        Trace = static_cast<U8>(Level::Trace),
        Warning = static_cast<U8>(Level::Warning),
        Error = static_cast<U8>(Level::Error)
    };
    
    constexpr LevelMask operator+(const LevelMask left, const LevelMask right)
    {
        return static_cast<LevelMask>(static_cast<U8>(left) | static_cast<U8>(right));
    }

    constexpr LevelMask operator+=(LevelMask& left, const LevelMask right)
    {
        reinterpret_cast<U8&>(left) |= static_cast<U8>(right);

        return left;
    }
    
    constexpr LevelMask operator-(const LevelMask left, const LevelMask right)
    {
        return static_cast<LevelMask>(static_cast<U8>(left) & ~static_cast<U8>(right));
    }

    constexpr LevelMask operator-=(LevelMask& left, const LevelMask right)
    {
        left = left - right;

        return left;
    }
    
    constexpr bool operator&(const LevelMask mask, const LevelMask query)
    {
        return static_cast<U8>(mask) & static_cast<U8>(query);
    }
    
    constexpr bool operator&(const LevelMask mask, const Level query)
    {
        return static_cast<U8>(mask) & static_cast<U8>(query);
    }
}