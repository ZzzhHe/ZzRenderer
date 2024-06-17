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
    sampler2D albedo; 
    sampler2D metallic;
	sampler2D roughness;
	sampler2D normal;
	sampler2D ao;
	
	sampler2D emission;
};

uniform bool hasEmissionMap;

uniform Material material;
uniform sampler2DArray shadowMap;

#define MAX_LIGHTS 10
#define farPlane 200.0
#define PI 3.14159265359

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

vec3 getNormal() {
	vec3 tangentNormal = texture(material.normal, fs_in.TexCoords).rgb * 2.0 - 1.0;
	return normalize(fs_in.TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec4 CalcDirectLight(DirectLight light, vec3 N, vec3 V, vec3 F0, float metallic, float roughness, vec3 albedo) {
	vec3 L = normalize(-light.direction);
	vec3 H = normalize(V + L);
	vec3 radiance = light.color.xyz * light.color.w;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	float NdotL = max(dot(N, L), 0.0);

	vec3 outColor = (kD * albedo / PI + specular) * radiance * NdotL;

	return vec4(outColor, 1.0);
}

vec4 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 F0, float metallic, float roughness, vec3 albedo) {
	vec3 L = normalize(light.position - fs_in.FragPos);
	vec3 H = normalize(V + L);
	float distance = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = light.color.xyz * light.color.w * attenuation;

	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	float NdotL = max(dot(N, L), 0.0);

	vec3 outColor = (kD * albedo / PI + specular) * radiance * NdotL;

	return vec4(outColor, 1.0);
}

float ShadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir) {
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
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float currentDepth = projCoords.z;

	if (currentDepth > 1.0) {
		return 0.0;
	}

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	const float biasModifier = 0.5f;
	if (layer == cascadeCount) {
		bias *= 1 / (farPlane * biasModifier);
	} else {
		bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
	}
	bias *= 0.1;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
	for (int x = -1; x <= 1; ++x) {
		 for (int y = -1; y <= 1; ++y) {
			 float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
			 shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
		 }
	}
	shadow /= 9.0;

	return shadow;
}

void main() {

	vec3 albedo     = pow(texture(material.albedo, fs_in.TexCoords).rgb, vec3(2.2));
	float metallic  = texture(material.metallic, fs_in.TexCoords).r;
	float roughness = texture(material.roughness, fs_in.TexCoords).r;
	float ao        = texture(material.ao, fs_in.TexCoords).r;

	vec3 N = getNormal();
	vec3 V = normalize(viewPos - fs_in.FragPos);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);
	
	vec3 Lo = vec3(0.0);

	for (int i = 0; i < numDirectLights; i ++) {
		vec3 lightDir = normalize(-directLight[i].direction);
		float shadow = ShadowCalculation(fs_in.FragPos, N, lightDir);
		Lo += CalcDirectLight(directLight[i], N, V, F0, metallic, roughness, albedo).rgb * (1 - shadow);
	}
//	
//	for (int i = 0; i < numPointLights; i ++) {
//		vec3 lightDir = normalize(pointLight[i].position - fs_in.FragPos);
//		float shadow = ShadowCalculation(fs_in.FragPos, N, lightDir);
//		Lo += CalcPointLight(pointLight[i], N, V, F0, metallic, roughness, albedo).rgb * (1.0 - shadow);
//	}
	
	vec3 ambient = vec3(0.05) * albedo * ao;
	
	// emission
	vec3 emission = vec3(0.0, 0.0, 0.0);
	if (hasEmissionMap) {
		emission = texture(material.emission, fs_in.TexCoords).rgb * 2.0f;
	}
	
	vec3 color = ambient + Lo + emission;
	
	FragColor = vec4(color, 1.0);
}
