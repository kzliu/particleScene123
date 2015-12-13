#version 400

layout(location = 0) in vec2 texCoord;

uniform sampler2D positions;
uniform sampler2D velocities;
uniform vec2 statedimensions;
uniform vec2 worlddimensions;
uniform float size;
uniform float pscale;
uniform float vscale;

out vec2 velocity;

const float BASE = 255.0;
const float OFFSET = BASE * BASE / 2.0;

void decode(in float dot_product, in float scale, out float val){
    val = (dot_product - OFFSET) / scale;
}

void main() {
    vec4 psample = texture2D(positions, texCoord / statedimensions);
    vec4 vsample = texture2D(velocities, texCoord / statedimensions);

    float dot_product1p = dot(psample.xy, vec2(BASE, BASE*BASE));
    float dot_product2p = dot(psample.zw, vec2(BASE, BASE*BASE));
    float px;
    float py;
    decode(dot_product1p, pscale, px);
    decode(dot_product2p, pscale, py);
    vec2 p = vec2(px,py);

    float dot_product1v = dot(vsample.xy, vec2(BASE, BASE*BASE));
    float dot_product2v = dot(vsample.zw, vec2(BASE, BASE*BASE));
    float vx;
    float vy;
    decode(dot_product1v, vscale, vx);
    decode(dot_product2v, vscale, vy);
    velocity = vec2(vx,vy);

    gl_Position = vec4(p / worlddimensions * 2.0 - 1.0, 0, 1);
    gl_PointSize = size;
}

