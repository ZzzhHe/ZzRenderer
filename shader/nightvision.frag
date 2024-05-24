#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D NightVisionTexture;
uniform float time; // Time variable for dynamic noise

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(NightVisionTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    // Convert to grayscale
    float gray = dot(col, vec3(0.299, 0.587, 0.114));

    // Apply green tint
    vec3 nightVisionColor = vec3(0.0, gray, 0.0);

    // Add noise
    float noise = (fract(sin(dot(TexCoords.xy * time, vec2(12.9898,78.233))) * 43758.5453) - 0.5) * 0.05;
    nightVisionColor += noise;

    // Adjust brightness and contrast
    nightVisionColor = clamp((nightVisionColor - 0.5) * 2.0 + 0.5, 0.0, 1.0);

    FragColor = vec4(nightVisionColor, 1.0);
}
