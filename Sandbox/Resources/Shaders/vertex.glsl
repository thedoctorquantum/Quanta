#version 450 core

layout(location = 0) in vec3 a_Translation;
layout(location = 1) in vec4 a_Color;

layout(location = 0) out Out
{
    vec4 color;
} v_Out;

void main()
{
    v_Out.color = a_Color;

    gl_Position = vec4(a_Translation, 1.0);
}