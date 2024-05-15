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

uniform sampler2D texture1;

uniform DirectLight directLight;

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec4 color);

void main() {
	
	vec3 norm = normalize(Normal);
    vec3 viewPos = vec3(2.0f, -3.0f, -10.0f);
	vec3 viewDir = normalize(viewPos - FragPos);
    vec4 color = texture(texture1, TexCoords);
    vec4 direct_light = CalcDirectLight(directLight, norm, viewDir, color);
	FragColor = direct_light;
}

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, vec4 color) {
	float alpha = color.a;
	
	vec3 lightDirection = normalize(-light.direction);

    vec3 lightColor = light.color.xyz * light.color.w;
	
	// ambient
	vec3 ambient = light.ambientColor.xyz * light.ambientColor.w; 
	
	// diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = lightColor * diff;
	
	// specular
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	vec3 specular = lightColor * spec;
	
	
	vec3 outColor = ambient + (diffuse + specular) * vec3(color);
		
	return vec4(outColor, alpha);
}