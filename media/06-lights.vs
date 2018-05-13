#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texcoord;

out vec2 v_texcoord;
out vec3 v_modelPosition;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_modelView;
uniform mat4 u_modelViewProj;

void main()
{
	v_texcoord = a_texcoord;
	
    v_modelPosition = vec3(u_model * vec4(a_position, 1.0));

	v_normal = mat3(transpose(inverse(u_modelView))) * a_normal;  

    gl_Position = u_modelViewProj * vec4(a_position, 1.0);
}