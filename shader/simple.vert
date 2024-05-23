#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VS_OUT {
    vec2 TexCoords;
    vec3 FragPos;
    mat3 TBN;
} vs_out;

layout (std140) uniform UboCamera {
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoord;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    // re-orthogonalize T with respect to N
    // T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    // vec3 B = cross(N, T);

    vs_out.TBN = mat3(T, B, N);
}