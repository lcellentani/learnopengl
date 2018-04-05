#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

out vec3 v_viewPosition;
out vec3 v_lightViewPosition;
out vec3 v_normal;

uniform mat4 u_view;
uniform mat4 u_viewModel;
uniform mat4 u_modelViewProj;
uniform vec3 u_lightPosition; 

void main()
{
    v_viewPosition = vec3(u_viewModel * vec4(a_position, 1.0));
    v_lightViewPosition = vec3(u_view * vec4(u_lightPosition, 1.0));
	
	v_normal = mat3(transpose(inverse(u_viewModel))) * a_normal;  

    gl_Position = u_modelViewProj * vec4(a_position, 1.0);
}