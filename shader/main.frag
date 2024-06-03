#version 410 core

in VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
	vec3 Normal;
//	vec4 FragPosLightSpace;
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
uniform sampler2DArray shadowMap;

#define MAX_LIGHTS 10
#define farPlane 200.0

uniform DirectLight directLight[MAX_LIGHTS];
uniform PointLight pointLight[MAX_LIGHTS];
uniform int numDirectLights;
uniform int numPointLights;

uniform int cascadeCount;
uniform mat4 lightSpaceMatrices[16];
uniform float cascadePlaneDistances[16];

layout (std140) uniform UboCamera {
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

vec4 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir, float shadow);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, float shadow);
float ShadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir);

void main() {
	vec3 normal = texture(material.normal, fs_in.TexCoords).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(fs_in.TBN * normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	
	vec4 direct_light_color = vec4(0.0);
	for (int i = 0; i < numDirectLights; i ++) {
		vec3 lightDir = normalize(-directLight[i].direction);
		float shadow = ShadowCalculation(fs_in.FragPos, normal, lightDir);
		direct_light_color += CalcDirectLight(directLight[i], normal, viewDir, shadow);
	}
	vec4 point_light_color = vec4(0.0);
//	for (int i = 0; i < numPointLights; i ++) {
//		point_light_color += CalcPointLight(pointLight[i], normal, viewDir, shadow);
//	}
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

float ShadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir) {
	// select cascade layer
	vec4 fragPosViewSpace = view * vec4(fragPos, 1.0);
	float depthValue = abs(fragPosViewSpace.z);

	int layer = -1;
	for (int i = 0; i < cascadeCount; ++i) {
		if (depthValue < cascadePlaneDistances[i]) {
			layer = i;
			break;
		}
	}
	if (layer == -1) {
		layer = cascadeCount;
	}

	vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPos, 1.0);
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (currentDepth > 1.0) {
		return 0.0;
	}
	// calculate bias (based on depth map resolution and slope)
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	const float biasModifier = 0.5f;
	if (layer == cascadeCount) {
		bias *= 1 / (farPlane * biasModifier);
	} else {
		bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
	}

	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
			shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
		
	return shadow;
}
