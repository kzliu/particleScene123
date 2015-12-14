#version 330

in vec4 gl_FragCoord;                   // location of fragment in window space

layout(location = 0) out vec4 color;             //out fragColor will be written into texture


// projections near and far plane
float near = 1.0;
float far  = 200.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    color = vec4(vec3(depth), 1.0f);
}
