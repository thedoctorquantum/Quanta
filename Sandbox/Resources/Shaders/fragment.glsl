#version 450 core

layout(location = 0) out vec4 a_Fragment;

layout(location = 0) in Out
{
    vec4 color;
} v_In;

void main()
{
    a_Fragment = v_In.color;
}