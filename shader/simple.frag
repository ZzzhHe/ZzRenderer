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

struct Material {
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D normal;
};

uniform Material material;

uniform DirectLight directLight;

uniform vec3 viewPos;

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
    vec4 direct_light_color = CalcDirectLight(directLight, norm, viewDir);
	FragColor = direct_light_color;
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
