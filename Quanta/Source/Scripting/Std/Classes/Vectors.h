#pragma once

#include <angelscript.h>
#include <glm/glm.hpp>

namespace Quanta::As_Std
{
    using vec2i32 = glm::i32vec2;
    using vec3i32 = glm::i32vec3;
    using vec4i32 = glm::i32vec4;

    using vec2u32 = glm::u32vec2;
    using vec3u32 = glm::u32vec3;
    using vec4u32 = glm::u32vec4;
    
    using vec2f32 = glm::f32vec2;
    using vec3f32 = glm::f32vec3;
    using vec4f32 = glm::f32vec4;

    void RegisterVectors(asIScriptEngine* engine);
}