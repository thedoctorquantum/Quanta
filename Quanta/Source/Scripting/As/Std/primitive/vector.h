#pragma once

#include <angelscript.h>
#include <glm/glm.hpp>

namespace Quanta::As::Std
{
    using Vec2F32 = glm::f32vec2;
    using Vec3F32 = glm::f32vec3;
    using Vec4F32 = glm::f32vec4;
    
    void RegisterVectors(asIScriptEngine* engine);
}