#version 400



in vec2 uv;
// TODO (Task 8): Declare a sampler2D uniform
uniform sampler2D position;
uniform sampler2D velocity;
//uniform float random;
//uniform int derivative;
uniform vec2 scale;
//uniform vec2 worlddimensions;

const int BASE = 255;
const int OFFSET = (BASE * BASE / 2);

out vec4 color;

//int encode(int value, int scale) {
//    value = value * scale + OFFSET;
//    int x = mod(value, BASE);
//    int y = floor(value, BASE);
//    return vec2(x,y) /BASE;
//}


//int decode(vec2 channels){
//    return (dot(channels, vec2(BASE, BASE*BASE)) - OFFSET) / scale;
//}


void main(){
    // TODO (Task 8): Set the output color to the corresponding texel
    vec4 psample = texture(position, uv);
    //    vec4 vsample = texture2D(velocity, index);
    //    vec2 p = vec2(decode(psample.rg, scale.x), decode(psample.ba, scale.x));
    //    vec2 v = vec2(decode(vsample.rg, scale.y), decode(vsample.ba, scale.y));
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
    //    fragColor = vec4(encode(result.x, s), encode(result.y, s));

    color = psample;
//    color = texture(texture1, uv);

//    color = vec4(100,255,255, 255);
}
