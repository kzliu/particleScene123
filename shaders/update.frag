#version 400

uniform sampler2D position;
uniform sampler2D velocity;
uniform float random;
uniform int derivative;
uniform float pscale;
uniform float vscale;
uniform vec2 worlddimensions;
in vec2 uv;

out vec4 gl_FragColor;

const int BASE = 255;
const int OFFSET = (BASE * BASE / 2);


void encode(in float value, in float scale, out vec2 xy) {
    value = value * scale + OFFSET;
    xy.x = mod(value, BASE) /BASE ;
    xy.y = floor(value / BASE) / BASE;
}



void decode(in float dot_product, in float scale, out float val){
    val = (dot_product - OFFSET) / scale;
}

void updatePosition(inout vec2 p, inout vec2 v){
    p += v;
    if (p.y <= 0.0 || p.x < 0.0 || p.x > worlddimensions.x) {
        /* Left the world, reset particle. */
        p.y += worlddimensions.y + random + (uv.y - 0.5) * sign(random);
        p.x = mod(p.x + random * 10.0, worlddimensions.x);
    }
}


void updateVelocity(inout vec2 p, inout vec2 v) {
//    v += gravity;
    v -= 0.3;
    if (p.y + v.y < -1.0) {
        /* Left the world, reset particle. */
        v.x = v.x + random / 2.0 + (uv.x - 0.5) * sign(random);
        v.y = 0.0;
    }
}


void main()
{
    vec4 psample = texture(position, uv);
    vec4 vsample = texture(velocity, uv);
//    (dot(channels, to_dot) <- dot product where channels = psample and csample .ba and .rg
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
    vec2 v = vec2(vx,vy);

    vec2 result;
    float s;
    if (derivative == 0) {
        updatePosition(p, v);
        result = p;
        s = pscale;
    } else {
        updateVelocity(p, v);
        result = v;
        s = vscale;
    }


    vec2 frag1, frag2;

    encode(result.x, s, frag1);
    encode(result.y, s, frag2);


    gl_FragColor = vec4(frag1.x, frag1.y, frag2.x, frag2.y);
//    gl_FragColor = vsample;


}
