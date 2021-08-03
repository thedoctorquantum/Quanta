#include <Quanta/Graphics/Imaging/Color32.h>

#include "../../Debugging/Validation.h"

namespace Quanta
{
    Color32::Color32(uint32_t value)
    {
        PackedValue = value;
    }

    Color32::Color32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        Red = red;
        Green = green;
        Blue = blue;
        Alpha = alpha;
    }

    uint8_t& Color32::operator[](size_t index)
    {
        DEBUG_ASSERT(index < 3);
        
        return RedGreenBlueAlpha[index];
    }
    
    const uint8_t& Color32::operator[](size_t index) const
    {
        DEBUG_ASSERT(index < 3);

        return RedGreenBlueAlpha[index];
    }
}