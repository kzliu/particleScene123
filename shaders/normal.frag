#version 330

in vec3 WorldSpace_normal;
in vec3 WorldSpace_position;
in vec2 uv;
in vec3 WorldSpace_tangent;  //world-space tangent

uniform sampler2D texture1;

layout(location = 0) out vec4 normal;             //out fragColor will be written into texture

void main(void)
{

    vec3 tangent = normalize(WorldSpace_tangent);
    tangent = normalize(tangent - dot(tangent, WorldSpace_normal.xyz) * WorldSpace_normal.xyz);
    vec3 Bitangent = cross(tangent, WorldSpace_normal.xyz);
    vec3 BumpMapNormal = texture2D(texture1, uv).yzw;
    BumpMapNormal = normalize(2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0));
    vec3 NewNormal;
    mat3 TBN = mat3(tangent, Bitangent, WorldSpace_normal.xyz);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);

//    vec3 Color = texture(texturestone, uv).xyz;

//    fragColor = Color;


    normal.r = NewNormal.x;
    normal.g = NewNormal.y;
    normal.b = NewNormal.z;

//    normal.r = WorldSpace_normal.x;
//    normal.g = WorldSpace_normal.y;
//    normal.b = WorldSpace_normal.z;

}
