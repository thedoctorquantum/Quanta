#pragma one

#include "Permission.h"

namespace Quanta::Shell
{
    enum struct PermissionMask : U8
    {
        None = 0,
        All = 1,

        Low = static_cast<U8>(Permission::Low),
        Medium = static_cast<U8>(Permission::Medium),
        High = static_cast<U8>(Permission::High)
    };

    constexpr PermissionMask operator+(const PermissionMask left, const PermissionMask right)
    {
        return static_cast<PermissionMask>(static_cast<U8>(left) | static_cast<U8>(right));
    }

    constexpr PermissionMask operator+=(PermissionMask& left, const PermissionMask right)
    {
        reinterpret_cast<U8&>(left) |= static_cast<U8>(right);

        return left;
    }
    
    constexpr PermissionMask operator-(const PermissionMask left, const PermissionMask right)
    {
        return static_cast<PermissionMask>(static_cast<U8>(left) & ~static_cast<U8>(right));
    }

    constexpr PermissionMask operator-=(PermissionMask& left, const PermissionMask right)
    {
        left = left - right;

        return left;
    }
    
    constexpr bool operator&(const PermissionMask mask, const PermissionMask query)
    {
        return static_cast<U8>(mask) & static_cast<U8>(query);
    }
    
    constexpr bool operator&(const PermissionMask mask, const Permission query)
    {
        return static_cast<U8>(mask) & static_cast<U8>(query);
    }
}