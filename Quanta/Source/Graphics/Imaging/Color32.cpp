#include <Quanta/Graphics/Imaging/Color32.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    Color32::Color32(U32 value)
    {
        PackedValue = value;
    }

    Color32::Color32(U8 red, U8 green, U8 blue, U8 alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    U8& Color32::operator[](USize index)
    {
        DEBUG_ASSERT(index < 3);
        
        return RedGreenBlueAlpha[index];
    }
    
    const U8& Color32::operator[](USize index) const
    {
        DEBUG_ASSERT(index < 3);

        return RedGreenBlueAlpha[index];
    }
}