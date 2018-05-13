#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(u_light.position - v_modelPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuse, v_texcoord));

	// specular
    vec3 viewDir = normalize(u_viewPosition - v_modelPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specular, v_texcoord));
    
    vec3 result = u_light.ambient + diffuse + specular;
    o_color = vec4(result, 1.0);
} 