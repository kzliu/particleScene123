#version 330

layout(location = 0) in vec3 ObjectSpace_position;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 ObjectSpace_normal;
layout(location = 3) in vec3 tang;


uniform mat4 model, view, projection;

out vec3 WorldSpace_position; // world-space position
out vec3 WorldSpace_normal;   // world-space normal

void main() {

    WorldSpace_position = (model * vec4(ObjectSpace_position, 1.0)).xyz;
    WorldSpace_normal = (model * vec4(ObjectSpace_normal, 0.0)).xyz;

    gl_Position = projection * view *  vec4(WorldSpace_position, 1.0);
}
