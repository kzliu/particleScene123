#version 400 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 inUV;

out vec2 uv;

void main() {

    uv = inUV;
    gl_Position = vec4(position, 1.0);

}

//layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 inUV;
//layout(location = 2) in vec3 normal;

//uniform mat4 model, view, projection;

//layout(location = 0) in vec3 ObjectSpace_position;
//layout(location = 1) in vec2 inUV;
//layout(location = 2) in vec3 ObjectSpace_normal;
//layout(location = 3) in vec3 tang;

//uniform mat4 model, view, projection;

//out vec3 WorldSpace_position; // world-space position
//out vec2 uv;
//out vec3 WorldSpace_normal;   // world-space normal
//out vec3 WorldSpace_tangent;

////out vec2 uv;

//void main() {
//    uv = inUV;

//    WorldSpace_position = (model * vec4(ObjectSpace_position, 1.0)).xyz;
//    WorldSpace_normal = (model * vec4(ObjectSpace_normal, 0.0)).xyz;


////    WorldSpace_tangent = (model * vec4(tang, 0.f))).xyz;

//    gl_Position = vec4(ObjectSpace_position, 1.0);


//}
