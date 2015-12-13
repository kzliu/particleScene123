uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

uniform sampler2D position
uniform sampler2D velocity;
uniform float random;
uniform int derivative;
uniform int pscale;
uniform int vscale;
uniform vec2 worlddimensions;
varying vec2 index;

const int BASE 255;
const int OFFSET (BASE * BASE / 2);


int encode(int value, int scale) {
    value = value * scale + OFFSET;
    int x = mod(value, BASE);
    int y = floor(value, BASE);
    return vec2(x,y) /BASE;
}


int decode(vec2 channels){
    return (dot(channels, vec2(BASE, BASE*BASE)) - OFFSET) / scale;
}

vec2 updatePosition(inout vec2 p, inout vec2 v){
    p += v;
    if (p.y <= 0.0 || p.x < 0.0 || p.x > worlddimensions.x) {
        /* Left the world, reset particle. */
        p.y += worlddimensions.y + random + (index.y - 0.5) * sign(random);
        p.x = mod(p.x + random * 10.0, worlddimensions.x);
    }
}

void updateVelocity(inout vec2 p, inout vec2 v) {
    v += gravity;
    if (p.y + v.y < -1.0) {
        /* Left the world, reset particle. */
        v.x = v.x + random / 2.0 + (index.x - 0.5) * sign(random);
        v.y = 0.0;
    }
}

void main(void)
{
    vec4 psample = texture2D(position, index);
    vec4 vsample = texture2D(velocity, index);
    vec2 p = vec2(decode(psample.rg, scale.x), decode(psample.ba, scale.x));
    vec2 v = vec2(decode(vsample.rg, scale.y), decode(vsample.ba, scale.y));
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
    gl_FragColor = vec4(encode(result.x, s), encode(result.y, s));

    //gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
