#pragma once

#include "../../Math/Integer.h"

namespace Quanta
{
    struct Color32 final
    {
        union 
        {
            U32 PackedValue = 0;
            
            U8 RedGreenBlueAlpha[4];
                        
            struct 
            {
                U8 Red;
                U8 Green;
                U8 Blue;
                U8 Alpha;
            };
        };
        
        U8& operator[](USize index);
        const U8& operator[](USize index) const;
        
        Color32() = default;

        Color32(U32 value);
        Color32(U8 red, U8 green, U8 blue, U8 alpha);
    };   
}