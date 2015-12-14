#version 330

layout(location = 0) in vec3 ObjectSpace_position;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 ObjectSpace_normal;
layout(location = 3) in vec3 tang;


uniform mat4 model, view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(ObjectSpace_position, 1.0);
}
