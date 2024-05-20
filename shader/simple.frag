#version 410 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct DirectLight {
    vec3 direction;
    vec4 ambientColor;
    vec4 color;
};

struct PointLight {
	vec3 position;
	vec4 color;
	vec4 ambientColor;
	float constant;
	float linear;
	float quadratic;
};

struct Material {
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D normal;
};

uniform Material material;

uniform DirectLight directLight;
uniform PointLight pointLight;

uniform vec3 viewPos;

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
    vec4 direct_light_color = CalcDirectLight(directLight, norm, viewDir);
	vec4 point_light_color = CalcPointLight(pointLight, norm, viewDir);
	FragColor = direct_light_color + point_light_color;
}

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir) {
	vec4 diffuseColor = texture(material.diffuse, TexCoords);
	vec4 specularColor = texture(material.specular, TexCoords);
	float alpha = diffuseColor.a;
	
	vec3 lightDirection = normalize(-light.direction);

    vec3 lightColor = light.color.xyz * light.color.w;
	
	// ambient
	vec3 ambient = light.ambientColor.xyz * light.ambientColor.w * vec3(diffuseColor);
	
	// diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * vec3(diffuseColor);
	
	// specular
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	vec3 specular = spec * vec3(specularColor);
	
	vec3 outColor = ambient + (diffuse + specular) * lightColor;
		
	return vec4(outColor, alpha);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec4 diffuseColor = texture(material.diffuse, TexCoords);
	vec4 specularColor = texture(material.specular, TexCoords);
	float alpha = diffuseColor.a;
	
	vec3 lightDirection = normalize(light.position - FragPos);

	vec3 lightColor = light.color.xyz * light.color.w;
	
	// ambient
	vec3 ambient = light.ambientColor.xyz * light.ambientColor.w * vec3(diffuseColor);
	
	// diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * vec3(diffuseColor);
	
	// specular
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	vec3 specular = spec * vec3(specularColor);
	
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 outColor = (ambient + (diffuse + specular) * lightColor) * attenuation;
		
	return vec4(outColor, alpha);
}
