#version 410 core

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
	vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

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
uniform sampler2D shadowMap;

#define MAX_LIGHTS 10

uniform DirectLight directLight[MAX_LIGHTS];
uniform PointLight pointLight[MAX_LIGHTS];
uniform int numDirectLights;
uniform int numPointLights;

layout (std140) uniform UboCamera {
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, float shadow);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float shadow);
float ShadowCalculation(vec4 fragPosLightSpace);

void main() {
	vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(fs_in.TBN * normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	vec4 direct_light_color = vec4(0.0);
	for (int i = 0; i < numDirectLights; i ++) {
		direct_light_color += CalcDirectLight(directLight[i], normal, viewDir, shadow);
	}
	vec4 point_light_color = vec4(0.0);
	for (int i = 0; i < numPointLights; i ++) {
		point_light_color += CalcPointLight(pointLight[i], normal, viewDir, shadow);
	}
	FragColor = direct_light_color + point_light_color;
}

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, float shadow) {
	vec4 diffuseColor = texture(material.diffuse, fs_in.TexCoords);
	vec4 specularColor = texture(material.specular, fs_in.TexCoords);
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
	
	vec3 outColor = ambient + (1.0 - shadow) * (diffuse + specular) * lightColor;
		
	return vec4(outColor, alpha);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float shadow) {
	vec4 diffuseColor = texture(material.diffuse, fs_in.TexCoords);
	vec4 specularColor = texture(material.specular, fs_in.TexCoords);
	float alpha = diffuseColor.a;
	
	vec3 lightDirection = normalize(light.position - fs_in.FragPos);

	vec3 lightColor = light.color.xyz * light.color.w;
	
	// ambient
	vec3 ambient = light.ambientColor.xyz * light.ambientColor.w * vec3(diffuseColor);
	
	// diffuse
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * vec3(diffuseColor);
	
	// specular
	vec3 halfwayDir = normalize(lightDirection + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0f);
	vec3 specular = spec * vec3(specularColor);
	
	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 outColor = (ambient + (1.0 - shadow) * (diffuse + specular) * lightColor) * attenuation;
		
	return vec4(outColor, alpha);
}

float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;	
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
//	FragColor = vec4(vec3(projCoords.z), 1.0);  //View depth values
//	FragColor = vec4(vec3(closestDepth), 1.0);
//	FragColor = vec4(vec3(currentDepth - bias), 1.0);

    return shadow;
}
