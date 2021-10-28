#version 450 core

layout(location = 0) out vec4 a_Fragment;

layout(location = 0) in Out
{
    vec4 color;
    vec2 uv;
} v_In;

layout(binding = 0) uniform sampler2D u_Sampler;

void main()
{
    a_Fragment = v_In.color * texture(u_Sampler, v_In.uv);
}