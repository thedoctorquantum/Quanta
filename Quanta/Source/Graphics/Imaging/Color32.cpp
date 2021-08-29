#include <Quanta/Graphics/Imaging/Color32.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    Color32::Color32(const U32 value)
    {
        PackedValue = value;
    }

    Color32::Color32(const U8 red, const U8 green, const U8 blue, const U8 alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    U8& Color32::operator[](const USize index)
    {
        DEBUG_ASSERT(index < 3);
        
        return RedGreenBlueAlpha[index];
    }
    
    const U8& Color32::operator[](const USize index) const
    {
        DEBUG_ASSERT(index < 3);

        return RedGreenBlueAlpha[index];
    }
}