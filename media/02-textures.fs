#version 330 core

out vec4 o_color;

in vec3 v_color;
in vec2 v_texcoord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main()
{
    o_color = mix(texture(u_texture1, v_texcoord), texture(u_texture2, v_texcoord), 0.2) * vec4(v_color, 1.0);
}