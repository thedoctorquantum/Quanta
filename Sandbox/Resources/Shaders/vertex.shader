#version 450 core

layout(location = 0) in vec3 a_Translation;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Uv;

layout(location = 0) out Out
{
    vec4 color;
    vec2 uv;
} v_Out;

layout(std140, binding = 0) uniform Uniforms
{
    mat4 model;
    mat4 projection;
} u_Uniforms;

void main()
{
    v_Out.color = a_Color;
    v_Out.uv = a_Uv;

    gl_Position = u_Uniforms.model * vec4(a_Translation, 1.0);
}