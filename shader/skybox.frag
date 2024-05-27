#version 410 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

const float offset = 1.0 / 300.0; // Adjust this value to control the blur radius

void main() {
    vec3 texColor = vec3(0.0);
    
    vec3 offsets[25] = vec3[](
        vec3(-2.0*offset,  2.0*offset, 2.0*offset), vec3(-offset,  2.0*offset, 2.0*offset), vec3(0.0, 2.0*offset, 2.0*offset), vec3(offset, 2.0*offset, 2.0*offset), vec3(2.0*offset, 2.0*offset, 2.0*offset),
        vec3(-2.0*offset,  offset, 2.0*offset),    vec3(-offset,  offset, 2.0*offset),    vec3(0.0, offset, 2.0*offset),    vec3(offset, offset, 2.0*offset),    vec3(2.0*offset, offset, 2.0*offset),
        vec3(-2.0*offset,  0.0, 2.0*offset),       vec3(-offset,  0.0, 2.0*offset),       vec3(0.0, 0.0, 2.0*offset),       vec3(offset, 0.0, 2.0*offset),       vec3(2.0*offset, 0.0, 2.0*offset),
        vec3(-2.0*offset, -offset, 2.0*offset),    vec3(-offset, -offset, 2.0*offset),    vec3(0.0, -offset, 2.0*offset),    vec3(offset, -offset, 2.0*offset),    vec3(2.0*offset, -offset, 2.0*offset),
        vec3(-2.0*offset, -2.0*offset, 2.0*offset), vec3(-offset, -2.0*offset, 2.0*offset), vec3(0.0, -2.0*offset, 2.0*offset), vec3(offset, -2.0*offset, 2.0*offset), vec3(2.0*offset, -2.0*offset, 2.0*offset)
    );

    float kernel[25] = float[](
        1.0/256, 4.0/256, 6.0/256, 4.0/256, 1.0/256,
        4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256,
        6.0/256, 24.0/256, 36.0/256, 24.0/256, 6.0/256,
        4.0/256, 16.0/256, 24.0/256, 16.0/256, 4.0/256,
        1.0/256, 4.0/256, 6.0/256, 4.0/256, 1.0/256
    );

    vec3 sampleTex[25];
    for(int i = 0; i < 25; i++) {
        sampleTex[i] = texture(skybox, TexCoords + offsets[i]).rgb;
    }
    vec3 color = vec3(0.0);
    for(int i = 0; i < 25; i++) {
        color += sampleTex[i] * kernel[i];
    }
        

    FragColor = vec4(color, 1.0);
}