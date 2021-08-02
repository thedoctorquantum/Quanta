#pragma once

#include <cstdint>
#include <cstddef>

namespace Quanta
{
    struct Color32 final
    {
        union 
        {
            uint32_t PackedValue = 0;
            
            uint8_t RedGreenBlueAlpha[4];
            
            struct 
            {
                uint8_t Red;
                uint8_t Green;
                uint8_t Blue;
                uint8_t Alpha;
            };
        };
        
        uint8_t& operator[](size_t index);
        const uint8_t& operator[](size_t index) const;
        
        Color32() = default;

        Color32(uint32_t value);
        Color32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    };   
}