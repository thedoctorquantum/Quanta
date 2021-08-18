#include <Quanta/Graphics/Imaging/Color32.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    Color32::Color32(UInt32 value)
    {
        PackedValue = value;
    }

    Color32::Color32(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    UInt8& Color32::operator[](Size index)
    {
        DEBUG_ASSERT(index < 3);
        
        return RedGreenBlueAlpha[index];
    }
    
    const UInt8& Color32::operator[](Size index) const
    {
        DEBUG_ASSERT(index < 3);

        return RedGreenBlueAlpha[index];
    }
}