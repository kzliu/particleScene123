#version 330

in vec3 WorldSpace_normal;
in vec3 WorldSpace_position;

layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 depth;

float near = 1.0;
float far  = 200.0;


float LinearizeDepth(float d)
{
    float z = d * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{

    position = (WorldSpace_position+15.f)/30.f;
    normal = WorldSpace_normal;

    float d = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    depth = vec4(vec3(d), 1.0f);
}
