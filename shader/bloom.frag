#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D BloomBlurMap;
uniform sampler2D scene;

void main() {
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(BloomBlurMap, TexCoords).rgb;
    vec3 finalColor = hdrColor + bloomColor; // additive blending
    FragColor = vec4(finalColor, 1.0);
}
