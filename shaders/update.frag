uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

uniform sampler2D position
uniform sampler2D velocity;
uniform int derivative;

in float scale;
in float random;

const float BASE 255.f;
const float OFFSET (BASE * BASE / 2.f);


float encode(float value) {
    value = value *scale + OFFSET;
    float x = mod(value, BASE);
    float y = floor(value, BASE);
    return vec2(x,y) /BASE;
}


float decode(vec2 channels){
    return (dot(channels, vec2(BASE, BASE*BASE)) - OFFSET) / scale;
}

vec2 updatePosition(){

}

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
