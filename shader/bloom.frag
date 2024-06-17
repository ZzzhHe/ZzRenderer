#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D BloomBlurMap;
uniform sampler2D scene;

const float exposure = 1.4f;
const float gamma = 2.2f;

void main() {
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(BloomBlurMap, TexCoords).rgb;
    vec3 finalColor = hdrColor + bloomColor; // additive blending
	// tone mapping
	vec3 result = vec3(1.0) - exp(-finalColor * exposure);
	// gamma correct
	result = pow(result, vec3(1.0 / gamma));
	FragColor = vec4(result, 1.0);
}
