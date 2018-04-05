#version 330 core
out vec4 o_color;

in vec3 v_viewPosition;
in vec3 v_lightViewPosition;
in vec3 v_normal;

uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(v_lightViewPosition - v_viewPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

	// specular
	float specularStrength = 0.5;
    vec3 viewDir = normalize(-v_viewPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * u_lightColor;  
	
		 
    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    o_color = vec4(result, 1.0);
} 