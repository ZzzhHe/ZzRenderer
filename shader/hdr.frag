#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D HDRTexture;

void main()
{             
    // const float gamma = 2.2;
    vec3 hdrColor = texture(HDRTexture, TexCoords).rgb;
    float exposure = 1.4f;
    // exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction 
    // mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}