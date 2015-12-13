#version 400

uniform sampler2D position;
uniform sampler2D velocity;
//uniform float random;
//uniform int derivative;
uniform int pscale;
//uniform int vscale;
//uniform vec2 worlddimensions;
in vec2 uv;

out vec4 gl_FragColor;

const int BASE = 255;
const int OFFSET = (BASE * BASE / 2);


void encode(in int value, in int scale, out vec2 xy) {
    value = value * scale + OFFSET;
    xy.x = mod(value, BASE) /BASE ;
    xy.y = floor(value / BASE) / BASE;
}


void decode(in int dot_product, in int scale, out int val){
    val = (dot_product - OFFSET) / scale;
}

//vec2 updatePosition(inout vec2 p, inout vec2 v){
//    p += v;
//    if (p.y <= 0.0 || p.x < 0.0 || p.x > worlddimensions.x) {
//        /* Left the world, reset particle. */
//        p.y += worlddimensions.y + random + (index.y - 0.5) * sign(random);
//        p.x = mod(p.x + random * 10.0, worlddimensions.x);
//    }
//}

//void updateVelocity(inout vec2 p, inout vec2 v) {
//    v += gravity;
//    if (p.y + v.y < -1.0) {
//        /* Left the world, reset particle. */
//        v.x = v.x + random / 2.0 + (index.x - 0.5) * sign(random);
//        v.y = 0.0;
//    }
//}

void main()
{
    vec4 psample = texture(position, uv);
    vec4 vsample = texture(velocity, uv);
//    (dot(channels, to_dot) <- dot product where channels = psample and csample .ba and .rg
//    vec2 p = vec2(decode(psample.rg, pscale), decode(psample.ba, pscale, vec2(BASE, BASE*BASE)));
//    vec2 v = vec2(decode(vsample.rg, vscale), decode(vsample.ba, vscale, vec2(BASE, BASE*BASE)));
//    vec2 result;
//    float s;
//    if (derivative == 0) {
//        updatePosition(p, v);
//        result = p;
//        s = pscale;
//    } else {
//        updateVelocity(p, v);
//        result = v;
//        s = vscale;
//    }
//    gl_FragColor = vec4(encode(result.x, s), encode(result.y, s));
    gl_FragColor = vsample;

}
