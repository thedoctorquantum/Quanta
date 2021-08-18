#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    struct Color32 final
    {
        union 
        {
            UInt32 PackedValue = 0;
            
            UInt8 RedGreenBlueAlpha[4];
            
            struct 
            {
                UInt8 Red;
                UInt8 Green;
                UInt8 Blue;
                UInt8 Alpha;
            };
        };
        
        UInt8& operator[](Size index);
        const UInt8& operator[](Size index) const;
        
        Color32() = default;

        Color32(UInt32 value);
        Color32(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha);
    };   
}