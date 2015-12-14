#version 400 core


//in vec3 WorldSpace_position; // world-space position
//in vec3 WorldSpace_normal;   // world-space normal
in vec3 WorldSpace_position; // world-space position
in vec2 uv;
in vec3 WorldSpace_normal;   // world-space normal
in vec3 WorldSpace_tangent;  //world-space tangent


uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D occlusion;
uniform sampler2D stone;            // stone texture
uniform sampler2D stoneUV;          // texture of uv coordinates


//uniform sampler2D texturestone;


uniform mat4 view, projection, model;

out vec3 fragColor;


void main(){

    vec3 color = texture(stone, texture(stoneUV,uv).xy).xyz;

    vec4 WorldSpace_position = (texture(position, uv)*30.f)-15.f;
    vec4 WorldSpace_normal = texture(normal, uv);
    float occlusion = texture(occlusion, uv).r;

    vec3 WorldSpace_toLight = normalize(vec3(10.0) - WorldSpace_position.xyz);


//    fragColor = (0.9 + 0.9 * max(0.0, dot(normal, WorldSpace_toLight)));

    fragColor = vec3(0.3*occlusion+ 0.7*max(0.0, dot(normalize(WorldSpace_normal.xyz), WorldSpace_toLight)));

}


