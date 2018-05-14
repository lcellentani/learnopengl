#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec4 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};

out vec4 o_color;

in vec2 v_texcoord;
in vec3 v_modelPosition;
in vec3 v_normal;

uniform vec3 u_viewPosition;
uniform Material u_material;
uniform Light u_light;

void main()
{
	vec3 norm = normalize(v_normal);
	vec3 viewDir = normalize(u_viewPosition - v_modelPosition);
	
	float attenuation = 1.0;
	vec3 lightDir = vec3(0.0, 0.0, 0.0);
	if (u_light.direction.w == 0) {
		lightDir = normalize(-u_light.direction.xyz);
	} else {
		lightDir = normalize(u_light.direction.xyz - v_modelPosition);
		float distance = length(u_light.direction.xyz - v_modelPosition);
		attenuation = 1.0 / (u_light.constant + u_light.linear * distance + u_light.quadratic * (distance * distance)); 
	}
	
	// diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, v_texcoord));

	// specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, v_texcoord));
    
    vec3 result = (u_light.ambient + diffuse + specular) * attenuation;
    o_color = vec4(result, 1.0);
} 