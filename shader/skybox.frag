#version 410 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main() {
	FragColor = vec4(pow(texture(skybox, TexCoords).rgb, vec3(2.2)), 1.0);
}
