#version 400 core

in vec3 WorldSpace_position; // world-space position
in vec2 uv;
in vec3 WorldSpace_normal;   // world-space normal
in vec3 WorldSpace_tangent;  //world-space tangent

uniform sampler2D texture1;
uniform sampler2D texturestone;

out vec3 fragColor;

void main(){
    vec3 WorldSpace_toLight = normalize(vec3(10.0) - WorldSpace_position);

    vec3 normal = normalize(WorldSpace_normal);

    vec3 tangent = normalize(WorldSpace_tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 Bitangent = cross(tangent, normal);
    vec3 BumpMapNormal = texture2D(texture1, uv).yzw;
    BumpMapNormal = normalize(2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0));
    vec3 NewNormal;
    mat3 TBN = mat3(tangent, Bitangent, normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);

    vec3 Color = texture(texturestone, uv).xyz;

    fragColor = Color;

    fragColor = (0.9 + 0.9 * max(0.0, dot(normal, WorldSpace_toLight)))*Color;



}


vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(WorldSpace_normal);

    vec3 Tangent = normalize(WorldSpace_tangent);
    Tangent = normalize(Tangent - dot(Tangent, normal) * normal);
    vec3 Bitangent = cross(Tangent, normal);
    vec3 BumpMapNormal = texture2D(texture1, uv).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}
