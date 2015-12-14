#version 330

layout(location = 0) in vec3 ObjectSpace_position;
layout(location = 1) in vec2 inUV;

out vec2 uv;

uniform mat4 model, view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(ObjectSpace_position, 1.0);
}
