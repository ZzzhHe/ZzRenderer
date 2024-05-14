#shader fragment
# version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D emission;
};

// Directional Light
struct DirectLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float intensity;
};
uniform DirectLight directLight;

// Light
uniform vec3 viewPos;
uniform Material material;


vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, float shadow);

void main() {
	
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
    vec4 direct_light = CalcDirectLight(directLight, norm, viewDir, shadow);
	FragColor = direct_light;
}

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, float shadow) {
	vec4 diffuseColor = texture(material.diffuse, TexCoords);
	vec4 specularColor = texture(material.specular, TexCoords);
	vec4 emissionColor = texture(material.emission, TexCoords);
	float alpha = diffuseColor.a;
	
	vec3 lightDirection = normalize(-light.direction);
	
	// ambient
	vec3 ambient = light.ambient * vec3(diffuseColor);
	
	// diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(diffuseColor);
	
	// specular
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	vec3 specular = light.specular * spec * vec3(specularColor);
	
	
	vec3 color = (ambient + diffuse + specular) * light.intensity;
		
	return vec4(color, alpha);
}