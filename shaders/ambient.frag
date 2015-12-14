#version 330

in vec2 uv;
out float fragColor;

uniform float width;
uniform float height;

const uniform int kernel_size = 64;
uniform vec3 kernel[64];

uniform sampler2D normal;       // in view/screen space
uniform sampler2D depth;
uniform sampler2D noise;
uniform mat4 model, view, projection;

void main()
{

    vec2 noiseScale = vec2(width/4.0, height/4.0); // factor to scale the noise texture by

    vec3 fragPosition = texture(depth, uv).xyz;
    vec3 fragNormal = (view * texture(normal, uv)).xyz;
    vec3 random = texture(noise, uv*noiseScale).xyz;

    vec3 tangent = normalize(random - fragNormal * dot(random, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);

    float occlusion = 0.0;
    float radius = 1.0;                     //radius is a parameter than can be tuned

    for(int i = 0; i < kernel_size; ++i)
    {
        // get sample position
        vec3 point = TBN * kernel[i];                       // transform to view space
        point = fragPosition + point * radius;
        vec4 offset = vec4(point, 1.0);
        offset = projection * offset;                       // from view to clip-space
        offset.xyz /= offset.w;                             // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5;                // transform to range 0.0 - 1.0

        float sampleDepth = texture(depth, offset.xy).z;    // depth of pixel at sample (x, y) location

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPosition.z - sampleDepth));
        occlusion += (sampleDepth >= point.z ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernel_size);            // occlusion = 1 -> no occlusion
    fragColor = occlusion;

}
