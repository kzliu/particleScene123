uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

uniform sampler2D position
uniform sampler2D velocity;
uniform int derivative;

const float BASE = 255.f;
const float OFFSET = BASE * BASE / 2.f;

float decode()


void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}
