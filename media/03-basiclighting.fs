#version 330 core
out vec4 o_color;

in vec3 v_position;
in vec3 v_normal;

  
uniform vec3 u_lightPosition; 
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPosition - v_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;
            
    vec3 result = (ambient + diffuse) * u_objectColor;
    o_color = vec4(result, 1.0);
} 